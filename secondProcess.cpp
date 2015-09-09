
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;
int thresh = 100;
int max_thresh = 255;
RNG rng(12345);
void CleanUpROI(Mat & src_){
    cvtColor(src_, src_, CV_BGR2HLS);
    for (int row=0;row<src_.rows;row++){
        for (int col=0;col<src_.cols;col++){
            uchar H = src_.at<Vec3b>(row,col)[0];
            uchar L = src_.at<Vec3b>(row,col)[1];
            uchar S = src_.at<Vec3b>(row,col)[2];
//                         double LS_ratio = ((double) L) / ((double) S);
//                         bool skin_pixel = (S >= 50) && (LS_ratio > 0.5) && (LS_ratio < 3.0) && ((H <= 14) || (H >= 165));
//                         if (!skin_pixel){
//                             imageROI.at<Vec3b>(row,col)[0]=0;
//                             imageROI.at<Vec3b>(row,col)[1]=0;
//                             imageROI.at<Vec3b>(row,col)[2]=0;
//                         }
        }}
    cvtColor(src_, src_, CV_HLS2BGR);

}
bool foo( float a, float b )
{
    std::cout<<"working"<<std::endl;
    return ( a != b );

}
void MainWindow::selectROI(Mat & src_,Mat & dst_,int thickness,bool rect){
    Mat bw1;
    vector<vector<Point> > contours1;
    vector<Vec4i> hierarchy1;

    /// Detect edges using Threshold
    cv::Canny(src_, bw1, 0, 10, 5);



    /// Find contours
    findContours( bw1, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    ///________________________CENTERS OF MASS______________________________________________
    /// Get the moments
    vector<Moments> mu(contours1.size() );
    vector<bool> centreRed(contours.size());
    for( int i = 0; i < contours1.size(); i++ )
     { mu[i] = moments( contours1[i], false ); }

    ///  Get the mass centers:
    vector<Point2f> mc( contours1.size() );
    Scalar color = Scalar( 255, 0, 0 );

    for( int i = 0; i < contours1.size(); i++ )
    {

        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
//        std::cout<<mc[i].x<<std::endl;
        putText(SrcRoi, ".", mc[i], 1, 2, color, 10 );

        if ( (mc[i].x + mc[i].y)>10  ) //bool check to see not NAN
        {

//            std::cout<<mc[i].x<<std::endl;
//            std::cout<< foo(mc[i].x,mc[i].y) <<std::endl;

            Vec3b color1 = SrcRoi_clean.at<Vec3b>(mc[i].x,mc[i].y);
            Vec3b color2 = SrcRoi_clean.at<Vec3b>(mc[i].x+2,mc[i].y+2);
            Vec3b color3 = SrcRoi_clean.at<Vec3b>(mc[i].x-2,mc[i].y-2);
            Vec3b color4 = SrcRoi_clean.at<Vec3b>(mc[i].x+1,mc[i].y);

            uchar r= color1[2];
            uchar g= color1[1];
            uchar b= color1[0];

            uchar r2= color2[2];
            uchar g2= color2[1];
            uchar b2= color2[0];

            uchar r3= color3[2];
            uchar g3= color3[1];
            uchar b3= color3[0];

            uchar r4= color4[2];
            uchar g4= color4[1];
            uchar b4= color4[0];


            if ( (r > b  && (r-b)>80  && r > g  && (r-g)>80 && r>50 ) ||
                 (r2 > b2  && (r2-b2)>80  && r2> g2  && (r2-g2)>80 && r2>50 ) ||
                 (r3 > b3  && (r3-b3)>80  && r3 > g3  && (r3-g3)>80 && r3>50 ) ||
                 (r4 > b4  && (r4-b4)>80  && r4 > g4  && (r4-g4)>80 && r4>50 )
                 )
            {
                centreRed[i]=true;
            }
            else
            {
                centreRed[i]=false;
            }

        }

    }
///_______________________________________________________________________________________
//    cv::findContours( src_, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


    /// Find the rotated rectangles and ellipses for each contour
    vector<Rect> minRect( contours1.size() );
    vector<RotatedRect> minEllipse( contours1.size() );

    for( int i = 0; i < contours1.size(); i++ ){
        //check size of min area rect for contours
        //check if centre is red
        if (! centreRed[i])
        {
            int b=(minAreaRect( Mat(contours1[i]) ).size.area());
            if ( b > minA && b < maxA )
            {
                if (rect){ minRect[i] = boundingRect( Mat(contours1[i]) );}
                if( contours1[i].size() > 5 ){ minEllipse[i] = fitEllipse( Mat(contours1[i]) ); }
            }
        }
    }


    /// Draw contours + rotated rects + ellipses
//    namedWindow("region",2);
    for( int i = 0; i< contours1.size(); i++ )
       {
        if (!centreRed[i]){
            int a=(minAreaRect( Mat(contours1[i]) ).size.area());
            if ( a > minA && a < maxA ){
                Scalar color = Scalar( 255, 255, 255 );
                // contour
                drawContours( dst_, contours1, i, color, 5, 8, vector<Vec4i>(), 0, Point() );
                // ellipse
                if (!rect){
                    ellipse( cnt_img, minEllipse[i], color, thickness, 8 );}
                if (rect){
                    RotatedRect re= RotatedRect(minEllipse[i].center,Size2f(minEllipse[i].size.width*1.2,minEllipse[i].size.height*1.2),minEllipse[i].angle);
                    ellipse( dst_, re, color, thickness, 8 );}
                // rotated rectangle
                if (rect){
                    rectangle( dst_, minRect[i].tl(), minRect[i].br(), color, 2, 8, 0 );
                    //get ROI, test ROI, printout Class
                    Rect r1=minRect[i];
                    Rect ro;
                    try{ro =Rect(r1.x,r1.y,r1.width,r1.height);}
                    catch (exception){cout<<"caught"<<endl;}

                    Mat imageROI= (SrcRoi_clean(minRect[i])).clone();
                    if (writeROI)
                    {
                        namedWindow("test",2);
                        imshow("test",imageROI);
                        waitKey(200);
                    }

 /* ROI SIZE*/

                    if (writeBackgrounds)
                    {
                        Mat imr;
                        try{imr= SrcRoi_clean(ro).clone(); cv::resize(imr,imr,size_,0,0,INTER_AREA);}
                        catch(exception){cout<<"Caught"<<endl;}

                        imwrite("/home/craig/QT/scripts/all_signs/"+std::to_string(name)+".png",imr);
                        name+=1;
                    }
//                    namedWindow("4",2);
//                    imshow("2",SrcRoi_clean);
                    Mat ri=imageROI.clone();
                    cv::resize(imageROI,imageROI,size_,0,0,INTER_LINEAR);
                    namedWindow("resize",2);

                    if (writeROI)
                    {
                        imwrite("/home/craig/Pictures/training_images/ROI/"+std::to_string(name)+".png",imageROI);
                        name+=1;
                    }
                    if (preProcessROI(imageROI)) //checks if center is red or not
                    {
                        getShape(imageROI);
                        imshow("resize",ri);
                        waitKey(100);
                    }
                    //                 CleanUpROI(imageROI);
                    //                 namedWindow("region",2);
                    //                 imshow("region",imageROI);
                    //                 Mat  img_mat3 = Mat(1,2700,CV_32FC1);
                    //                 string path_ ="/home/craig/scripts/temp/0 fuck.jpg";
                    //                 trainer.getFeatureRow(path_,img_mat3,0);
                    ///_____________________________________________________________________________PREDICTIONS WITH SVM_______________________________
                    //                 Mat img_mat4 =Mat(1,2700,CV_32FC1);
                    ////                 Mat tester=imread("/home/craig/scripts/training/110/1.jpg");
                    //                 trainer.makeFeatureRow(imageROI,img_mat4);

                    ////                 trainer.io.print_out_Mat(img_mat3);
                    //                 cout<<"\n  ______ \n "<<endl;
                    ////                 trainer.io.print_out_Mat(img_mat4);

                    //                 float answer1= svm.predict(img_mat4);
                    //                 cout<<answer1<<endl;
                    if (writeBackgrounds)
                    {

                    }
                    //                 waitKey(100);
                }
            }
//             Point2f rect_points[4]; minRect[i].points( rect_points );
//             for( int j = 0; j < 4; j++ )
//                line( dst_, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );}
       }}


}
