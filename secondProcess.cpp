
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

    ///________________________CENTERS OF MASS OF CONTOURS______________________________________________
    /// Get the moments
    vector<Moments> mu(contours1.size() );
    vector<bool> centreRed(contours.size());
    for( int i = 0; i < contours1.size(); i++ )
     { mu[i] = moments( contours1[i], false ); }

    ///  Get the mass centers:
    vector<Point2f> mc( contours1.size() );
    Scalar color = Scalar( 255, 0, 0 );

    ///search_____________________________________________________________________________
    vector<Point2f> pointsForSearch; //Insert all 2D points to this vector



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
                pointsForSearch.push_back(mc[i]);
            }

        }

    }
    ///search____________________________________________________________________________
    //iterate through all centres of mass. and querie closest points to each point
    //  add a group to a vector if more than 1 NN
    // search vector group to see if current point exists in group. (skip if yes, search if no)
    int range=50;
    int numOfPoints=5;
    flann::KDTreeIndexParams indexParams;
    flann::Index kdtree(Mat(pointsForSearch).reshape(1), indexParams);
    vector<float> query;
    query.push_back(pnt.x); //Insert the 2D point we need to find neighbours to the query
    query.push_back(pnt.y); //Insert the 2D point we need to find neighbours to the query
    vector<int> indices;
    vector<float> dists;
    kdtree.radiusSearch(query, indices, dists, range, numOfPoints);
    ///___________________________________________________________________________________
///_______________________________________________________________________________________
//    cv::findContours( src_, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );


    /// Find the rotated rectangles and ellipses for each contour
    /// first check if contours are close (for far away signs)

    vector<Rect> minRect( contours1.size() );
    vector<RotatedRect> minEllipse( contours1.size() );

    for( int i = 0; i < contours1.size(); i++ )
    {
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
            if ( a > minA && a < maxA )
            {
                Scalar color = Scalar( 255, 255, 255 );
                // contour
                drawContours( dst_, contours1, i, color, 5, 8, vector<Vec4i>(), 0, Point() );
                // ellipse
                if (!rect){
                    ellipse( cnt_img, minEllipse[i], color, thickness, 8 );}



                if (rect){
                    //draw ellipse
                    RotatedRect re= RotatedRect(minEllipse[i].center,Size2f(minEllipse[i].size.width*1.2,minEllipse[i].size.height*1.2),minEllipse[i].angle);
                    ellipse( dst_, re, color, thickness, 8 );
                //_____________________________________________________________________________________
                //draw rectangle
                    rectangle( dst_, minRect[i].tl(), minRect[i].br(), color, 2, 8, 0 );
                    //get ROI, test ROI, printout Class
                    Rect r1=minRect[i];
                    Rect ro;
                    try{ro =Rect(r1.x,r1.y,r1.width,r1.height);}
                    catch (exception){cout<<"caught rectangle creation exception"<<endl;}

                    Mat imageROI= (SrcRoi_clean(minRect[i])).clone();
                    Mat q= SrcRoi_clean.clone();


 /* ROI SIZE*/

                    Mat ri=imageROI.clone();

                    cv::resize(imageROI,imageROI,size_,0,0,INTER_NEAREST);

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
                    bool tri;
                    //check centre is not red, then find shape classify for the rectangle(roi and position)
                    if (preProcessROI(imageROI) && bitwise_shape) //checks if center is red or not
                    {
                        try{tri=getShape(imageROI,ri);}//true if triangle, false if circle
                        catch(exception e){ cout<<"cought an exception" << "is the template ROI same size as roi: (size_):"<<size_<<endl;}
                        namedWindow("roi",2);
                        cout<<"triangle is "<<tri<<endl;
//                        imshow("roi",ri);
//                        waitKey(timeout);
//                        imshow("roi",imageROI);
//                        waitKey(timeout);
                    }
                    // get centre position of rect in source image
                    if (checkedCenter && showdetections)
                    {
                        Mat ir1= (SrcRoi_clean(minRect[i]));
                        Size s;
                        Point centre;
                        ir1.locateROI(s,centre);
                        string label;
                        (tri) ? (label="tri") : (label="circle") ;

                        signs.add(centre,label,frameNo);
                        //check is any labels are set yet
                        map<int,string> mp=signs.checklabels();
                        map<int,string>::iterator i;
                        if (mp.size()>1){
                            for (i=mp.begin();i!=mp.end();i++)
                            {
                                //set image at position using key
                                int key = i-> first;
                                string val=i->second;
                                Point pos=signs.locations[key];
                                cout<<pos.x<<" "<<pos.y<<endl;
                                cout<<q.cols<<" "<<q.rows<<endl;
                                cout<<triangleSign.channels()<<" "<<q.channels()<<endl;
                                (val=="triangle") ? (triangleSign.copyTo(q(Rect(pos.x,pos.y,triangleSign.rows,triangleSign.cols)))) : (circleSign.copyTo(q(Rect(pos.x,pos.y,circleSign.rows,circleSign.cols)))) ;

                                cout<<"value: "<<val<<endl;
                            }

                            int fontface = cv::FONT_HERSHEY_SIMPLEX;
                            double scale = 2;
                            int thickness = 3;
                            cv::putText(q,to_string(frameNo),Point (100,100), fontface, scale, CV_RGB(0,0,255), thickness, 8);
                            namedWindow("roi",2);
                            imshow("roi",q);
                            waitKey(timeout);
                        }

                    }
//                    imshow("roi",SrcRoi_clean);

                    //use position to find last shape in same position.
                    //add new shape to the object.
                    //if object has 5 same shapes, classify(bring up image)
                    //object can contain areas for possible shapes
                    //object can contain each areas past shape classifications..
                    //object can handle showing sign on screen
                    //object can search its current items and compare based on x,y coordinates
                    //every time a position is checked, the object will update position and give a new label.
                    //object will remove possibles if 10 frames pass without close detection

                }
            }
        }
    }
}
