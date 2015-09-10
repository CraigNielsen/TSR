
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <time.h>
using namespace std;

void MainWindow::cropping(){
    //Check new floodfill mask match for a correct patch.
    //Get all points detected for minimal rotated Rect
    vector<Point> pointsInterest;
    Mat_<uchar>::iterator itMask= img_roiMask.begin<uchar>();
    Mat_<uchar>::iterator end= img_roiMask.end<uchar>();
    for( ; itMask!=end; ++itMask)
        if(*itMask==255)
            pointsInterest.push_back(itMask.pos());
    RotatedRect minRect = minAreaRect(pointsInterest);
//    drawRectangle(minRect,src);
//    showImage(src,"testing1 in cropping function");
    if(/*checkSizes(minRect)*/true){                                                    ///at moment looks for one rotated rect
                                                                                ///for all points..can seperate into multiple if want
                                                                                ///Also issue here, need to seperate signs
        //Get rotation matrix
        float r= (float)minRect.size.width / (float)minRect.size.height;
        float angle=minRect.angle;
        if(r<1)
            angle=90+angle;
        Mat rotmat= getRotationMatrix2D(minRect.center, angle,1);
        //Create and rotate image
        Mat img_rotated;
        warpAffine(src, img_rotated, rotmat, src.size(), CV_INTER_CUBIC);       ///changed to use src image here
        //Crop image
        Size rect_size=minRect.size;
        if(r < 1)
            swap(rect_size.width, rect_size.height);
        Mat img_crop;
        getRectSubPix(img_rotated, rect_size, minRect.center, img_crop);


        ////////                                                                INTERMEDIATE STEP YO


        croppedImageResultResized.create(300,900, CV_8UC3);
        cv::resize(img_crop, croppedImageResultResized, croppedImageResultResized.size(), 0, 0,
               INTER_CUBIC);
        //Equalize cropped image
                                                                                ///IMPORTANT TO change the UI to include manipulations for the following
        Mat grayResult;
        cvtColor(croppedImageResultResized, grayResult, CV_BGR2GRAY);
        equalizeHist(grayResult, grayResult);
        blur(grayResult, grayResult, Size(3,3));

        bitwise_not ( grayResult, grayResult );

        threshold(grayResult,grayResult,thresh_Low,thresh_High,0);

//        invertGrayImage(grayResult);
        finalCrop(grayResult);
//        showImage(grayResult,"final cropped image");



//        output.push_back(Plate(grayResult,minRect.boundingRect()));
    }

}

void MainWindow::finalCrop(Mat &image)
{
    int width=20;
    cv::Rect myROI(width, width, image.cols-2*width,image.rows-2*width);//top x, top y, top width , top height
    cv::Mat croppedImage;
    cv::Mat(image, myROI).copyTo(croppedImage);
    croppedImage.copyTo(image);
//    showImage(croppedImage,"testing the crop");

}
bool testCentre(Mat & src_)
{

    int rws=src_.rows/2;
    int cls=src_.cols/2;
    int color=(int)src_.at<uchar>(rws,cls);
    if (color == 255){return false;}
    else {return true;}

}
bool MainWindow::preProcessROI(Mat &src_)
{
    // Floodfill corners
    //    floodFill(src_,Point (2,2),255,0,50,50);
    getRed_inRGB(src_);

    //    src_gray= blank.clone();
    cv::inRange(src_, cv::Scalar(20, 20, 20), cv::Scalar(255, 255, 255), src_);
    Mat blank=Mat::zeros(SrcRoi.rows,SrcRoi.cols,CV_8UC1);
//    cout<< src_.channels()<<endl;
    //    cv::cvtColor(src_,blank,CV_BGR2GRAY);


    src_=src_>80;

//    cv::GaussianBlur(src_,src_,Size(1,1),1);
//    if (size_.height>=30){
//        Mat element = getStructuringElement( kernalType, Size( morph_width, morph_height )/*, Point( morph_size, morph_size )*/ );
//        //    morphologyEx(src_,src_,MORPH_CLOSE,element);
//        morphologyEx( src_, src_,  MORPH_OPEN, element );   //output is  (Src is always RGB)
//    }
    src_=src_>80;

    bool yes=testCentre(src_);
    if (yes)
    {
        if (writeROI)
        {
            namedWindow("floodfill",2);
            imshow("floodfill",src_);
            imwrite("/home/craig/Pictures/training_images/BW_ROIsmall/"+std::to_string(name)+".png",src_);
            name+=1;
        }
    }
    return yes;
//    waitKey(100);
    //threshold red out
    // treshold black near the centre
    // add red and black together and view to check

}
void MainWindow::floodfillProcess(){

    for(int i=0; i< rects.size(); i++){
        //For better rect cropping for each possible box

        //And then we can retrieve more clearly the contour box
        src.copyTo(img_seedPoints);
        src.copyTo(img_extractFromHere);

        circle(img_seedPoints, rects[i].center, 1, Scalar(0,255,0), -1);
//        showImage(img_seedPoints,"img_seedpoints");

        //get the min size between width and height
        // Circle Parameters:

        //    img – Image where the circle is drawn.
        //    center – Center of the circle.
        //    radius – Radius of the circle.
        //    color – Circle color.
        //    thickness – Thickness of the circle outline, if positive. Negative thickness means that a filled circle is to be drawn.
        //    lineType – Type of the circle boundary. See the line() description.
        //    shift – Number of fractional bits in the coordinates of the center and in the radius value.

        float minSize=(rects[i].size.width < rects[i].size.height)?
                    rects[i].size.width:rects[i].size.height;
        minSize=minSize-minSize*0.5;
        //initialize rand and get 5 points around center for floodfill algorithm
        srand ( time(NULL) );
        //Initialize floodfill parameters and variables


        if (!img_roiMask.data){
        img_roiMask.create(img_extractFromHere.rows + 2, img_extractFromHere.cols + 2, CV_8UC1);
        img_roiMask= Scalar::all(0);}

        int loDiff = 30;
        int upDiff = 30;
        int connectivity = 4;
        int newMaskVal = 255;
        int NumSeeds = 5;
        Rect ccomp;
        int flags = connectivity + (newMaskVal << 8 ) +
                CV_FLOODFILL_FIXED_RANGE + CV_FLOODFILL_MASK_ONLY;
        for(int j=0; j<NumSeeds; j++){
            Point seed;
            seed.x=rects[i].center.x+rand()%(int)minSize-(minSize/2);
            seed.y=rects[i].center.y+rand()%(int)minSize-(minSize/2);
            circle(img_seedPoints, seed, 1, Scalar(0,255,255), -1);
            int area = floodFill(img_extractFromHere, img_roiMask, seed, Scalar(255,0,0), &ccomp,
                                 Scalar(loDiff, loDiff, loDiff), Scalar(upDiff, upDiff, upDiff),
                                 flags);
        }

//        showImage(img_roiMask,"final Mask");

    }
}
