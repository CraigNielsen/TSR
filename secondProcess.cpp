
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
    return ( a != b );

}
void MainWindow::getContours(vector<vector<Point> > & contours1)
{
    Mat bw1;

    vector<Vec4i> hierarchy1;

    /// Detect edges using Threshold
//    cv::Canny(final, bw1, 0, 10, 5);
    cv::cvtColor(final, bw1, CV_BGR2GRAY);
//    namedWindow("final",2);
//    imshow("final",bw1);
//    waitKey(0);

    Mat cont=bw1.clone();
    /// Find contours
    findContours( cont, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    ///________________________CENTERS OF MASS OF CONTOURS______________________________________________
    if (contours1.size()>1)
    {

        vector<Point2f> centresOfMass( contours1.size() );
        map<int,vector<Point2f>> closeContours;
        cHandler.getMassCentres(contours1,centresOfMass);

//        cHandler.removeContourForCentreWithColour(SrcRoi_clean,centresOfMass,contours1);//only remove contours later on  when full contours are built
        cHandler.removeContourswithArea(contours1,maxA);
        cHandler.getCloseContours(centresOfMass,pointDistance,closeContours);
        if (closeContours.size()>1)
        {
            cHandler.drawCloseContours(centresOfMass,contours1,closeContours,bw1);
            //now that contours are drawn. find new contours. and repeat
        }
    }
    /// Find contours
    cont=bw1.clone();
    findContours( cont, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    if (contours1.size()>1)
    {

        vector<Point2f> centresOfMass( contours1.size() );
        map<int,vector<Point2f>> closeContours;
        cHandler.getMassCentres(contours1,centresOfMass);

//        //only remove contours later on  when full contours are built
        cHandler.removeContourswithArea(contours1,maxA);
        cHandler.getCloseContours(centresOfMass,pointDistance,closeContours);
        if (closeContours.size()>1)
        {
            cHandler.drawCloseContours(centresOfMass,contours1,closeContours,bw1);
            //now that contours are drawn. find new contours. and repeat
        }
        cHandler.removeContourForCentreWithColour(SrcRoi_clean,centresOfMass,contours1);
    }


    ///find contours again for the new combined contours in close regions
//    namedWindow("w",2);
//    imshow("w",bw1);
//    waitKey(0);
    findContours( bw1, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

}


void MainWindow::selectROI(vector<vector<Point> > & contours1,vector<Rect> & minRect){

    ///hand it a full contours and empty rectangle(that the function will fill)
    /// find areas around contours:
    Mat outImage=SrcRoi;
    Scalar color = Scalar( 255, 255, 255 );

    vector<RotatedRect> minEllipse( contours1.size() );

    for( int i = 0; i < contours1.size(); i++ )
    {

        int b=(minAreaRect( Mat(contours1[i]) ).size.area());
        if ( b > 0 )
        {
            Rect c=boundingRect( Mat(contours1[i]) );
            if (c.area()==0)
            {
                cout<<"error in size of bounding rectangle";
            }
            else {minRect[i] = c;}
            if( contours1[i].size() > 5 ){ minEllipse[i] = fitEllipse( Mat(contours1[i]) ); }
        }

    }


    /// Draw contours + rotated rects + ellipses

    for( unsigned int i = 0; i< contours1.size(); i++ )
    {
        int a=(minAreaRect( Mat(contours1[i]) ).size.area());
        if ( a > minA && a < maxA )
        {
            // contour
//            drawContours( outImage, contours1, i, color, 5, 8, vector<Vec4i>(), 0, Point() );
            // ellipse
            ellipse( cnt_img, minEllipse[i], color,4, 8 );
        }
    }
}



void MainWindow::classifyShape(vector<vector<Point> > & contours1,vector<Rect> minRect,Mat & frame)
    {
    for( unsigned int i = 0; i< contours1.size(); i++ )
    {

        Scalar color = Scalar( 255, 255, 255 );
        int a=(minAreaRect( Mat(contours1[i]) ).size.area());

        if ( a > minA && a < maxA )
        {

            //draw ellipse
//            RotatedRect re= RotatedRect(minEllipse[i].center,Size2f(minEllipse[i].size.width*1.2,minEllipse[i].size.height*1.2),minEllipse[i].angle);
//            ellipse( dst_, re, color, thickness, 8 );
            //_____________________________________________________________________________________
            //draw rectangle
            rectangle( SrcRoi, minRect[i].tl(), minRect[i].br(), color, 2, 8, 0 );
            //get ROI, test ROI, printout Class
            Rect r1=minRect[i];
            Rect ro;
            try{ro =Rect(r1.x,r1.y,r1.width,r1.height);}
            catch (exception){cout<<"caught rectangle creation exception"<<endl;}

            Mat imageROI= (SrcRoi_clean(minRect[i])).clone();
//            Mat q= SrcRoi_clean.clone();
            Mat q= SrcRoi;


            Mat ri=imageROI.clone();
            db(1.3);
            cv::resize(imageROI,imageROI,size_,0,0,INTER_NEAREST);
            db(1.4);
            Mat temp=imageROI.clone();
            bool checkedCenter=preProcessROI(temp);
            if (checkedCenter && writeROI)
            {
                namedWindow("writing",2);
                imshow("writing",imageROI);
                waitKey(200);
                imwrite(roiPath+std::to_string(name)+".png",imageROI);
                name+=1;
            }
            bool isSign;
            //check centre is not red, then find shape classify for the rectangle(roi and position)
            bool checkC=preProcessROI(imageROI);
            db(1.41);
            string type,sign;
            if (checkC && bitwise_shape) //checks if center is red or not
            {

                try{isSign=binaryShape(imageROI,type);
                sign=getShape(imageROI);}//true if triangle, false if circle
                catch(exception e){ cout<<"cought an exception" << "is the template ROI same size as roi: (size_):"<<size_<<endl;}
            }
            db(1.42);
            if (sign=="notSign"){cout<<"caught non sign"<<endl;continue;}
            // get centre position of rect in source image
            if (checkC && showdetections && isSign)
            {

                Mat ir1= (SrcRoi_clean(minRect[i]));
                Size s;
                Point centre;
                ir1.locateROI(s,centre);
                db(1.43);

                signs.add(centre,sign,frameNo);
                //check is any labels are set yet
                map<int,string> mp=signs.checklabels();
                map<int,string>::iterator i;
                db(1.5);
                //go through the map of detected signs and add an image to the frame passed into the function
                // can also add the image to a holder at the bottom of the window.
                if (mp.size()>0){
                    for ( i=mp.begin();i!=mp.end();i++)
                    {
                        //set image at position using key
                        int key = i-> first;
                        string val=i->second;
                        Point pos=signs.locations[key];
                        int iHeight= triangleSign.rows;
//                        cout<<"is :" <<pos.x + triangleSign.cols <<"> "<< frame.cols<<"? or is  "<<pos.y<<" > "<<frame.cols+triangleSign.cols<<endl;
//                        if (val=="utri")
//                        {
//                            (utriSign.copyTo(frame(Rect(pos.x+iHeight,pos.y,triangleSign.rows,triangleSign.cols))));
//                            continue;
//                        }
                        cout<< val<< "at x:" << pos.x <<" y: "<< pos.y << endl;
                         if ((pos.x+triangleSign.cols )> frame.cols)
                        {
                            (val=="triangle") ? (triangleSign.copyTo(frame(Rect(pos.x-triangleSign.cols-1,pos.y,triangleSign.rows,triangleSign.cols)))) : (circleSign.copyTo(frame(Rect(pos.x-circleSign.rows-1,pos.y,circleSign.rows,circleSign.cols)))) ;
                        }
                        else if ((pos.y+triangleSign.rows )> frame.rows)
                        {
                            (val=="triangle") ? (triangleSign.copyTo(frame(Rect(pos.x-1,pos.y+iHeight-triangleSign.rows,triangleSign.rows,triangleSign.cols)))) : (circleSign.copyTo(frame(Rect(pos.x-1,pos.y-triangleSign.rows,circleSign.rows,circleSign.cols)))) ;
                        }
                        else if (pos.x-iHeight > 0 )(val=="triangle") ? (triangleSign.copyTo(frame(Rect(pos.x-iHeight,pos.y,triangleSign.rows,triangleSign.cols)))) : (circleSign.copyTo(frame(Rect(pos.x-iHeight,pos.y,circleSign.rows,circleSign.cols)))) ;

                        else (val=="triangle") ? (triangleSign.copyTo(frame(Rect(pos.x+iHeight,pos.y,triangleSign.rows,triangleSign.cols)))) : (circleSign.copyTo(frame(Rect(pos.x+iHeight,pos.y,circleSign.rows,circleSign.cols)))) ;
//                        (val=="triangle") ? (triangleSign.copyTo(frame(Rect(0,0,triangleSign.rows,triangleSign.cols)))) : (circleSign.copyTo(frame(Rect(0,0,circleSign.rows,circleSign.cols)))) ;
                    }

//                    int fontface = cv::FONT_HERSHEY_SIMPLEX;
//                    double scale = 2;
//                    int thickness = 3;
//                    cv::putText(frame,to_string(frameNo),Point (100,100), fontface, scale, CV_RGB(0,0,255), thickness, 8);
//                    namedWindow("contours2",2);
//                    imshow("contours2",frame);

                }
                db(1.6);
            }
        }
    }
}
