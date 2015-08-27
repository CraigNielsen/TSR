
/**
 * Simple shape detector program.
 * It loads an image and tries to find simple shapes (rectangle, triangle, circle, etc) in it.
 * This program is a modified version of `squares.cpp` found in the OpenCV sample dir.
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cmath>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

/**
 * Helper function to display text in the center of a contour
 */
bool MainWindow::checkcenter(const Rect * Rect1,const Rect * Rect2){
    return Rect1->height < Rect2->height;
}
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 0.4;
    int thickness = 1;
    int baseline = 0;

    cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
    cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
    cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), CV_FILLED);
    cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}
struct top_to_bottom
{
    inline bool operator() (const RotatedRect& struct1, const RotatedRect& struct2)
    {
        return (struct1.size.height > struct2.size.height);
    }
};
void MainWindow::detectROIShapes_one()
{
/*creates a vector of points
 *(approximates a polygon curve using the chosen precision approxPolyDP
 * Ramer–Douglas–Peucker algorithm)
 * runs through the countours, approximates the number of sides..
 * creates region of Interest rectangles and adds to ROIRects
*/

        ROIrects.clear();

        std::vector<cv::Point> approx;
        for (int i = 0; i < contours.size(); i++)
        {
            // Approximate contour with accuracy proportional
            // to the contour perimeter
            cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

            // Skip small or non-convex objects
            if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
                continue;

            if (approx.size() == 3)
            {
                setLabel(idShapes, "TRI", contours[i]);    // Triangles
                RotatedRect R=minAreaRect(contours[i]); // Get bounding box for contour i
                R.size.height=R.size.height+R.size.height/3.;
                R.size.width=R.size.width+R.size.width/3.;
                ROIrects.push_back(R);
                //            Mat ROI=contourImage(R); //Set ROI on source image
                //            imshow("roi",ROI);
            }

            else if (approx.size() >= 4 && approx.size() <= 6)
            {
                // Number of vertices of polygonal curve
                int vtc = approx.size();

                // Get the cosines of all corners
                std::vector<double> cos;
                for (int j = 2; j < vtc+1; j++)
                    cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

                // Sort ascending the cosine values
                std::sort(cos.begin(), cos.end());

                // Get the lowest and the highest cosine
                double mincos = cos.front();
                double maxcos = cos.back();

                // Use the degrees obtained above and the number of vertices
                // to determine the shape of the contour
                if (vtc == 4 && mincos >= -0.2 && maxcos <= 0.3){
                    setLabel(idShapes, "RECT", contours[i]);
                    RotatedRect R=minAreaRect(contours[i]); // Get bounding box for contour i
                    if ((R.size.height/R.size.width)<10 && (R.size.width/R.size.height)<10)
                        ROIrects.push_back(R);}
                else if (vtc == 5 && mincos >= -0.4 && maxcos <= -0.20)
                    setLabel(idShapes, "PENTA", contours[i]);
                else if ( vtc <= 8 && vtc >= 6 && mincos >= -0.8 && maxcos <= -0.30)
                    setLabel(idShapes, "HEXA", contours[i]);
            }
            else
            {
                // Detect and label circles
                double area = cv::contourArea(contours[i]);
                cv::Rect r = cv::boundingRect(contours[i]);
                int radius = r.width / 2;

                if (std::abs(1 - ((double)r.width / r.height)) <= 0.2 &&
                        std::abs(1 - (area / (CV_PI * std::pow(radius, 2)))) <= 0.2)
                    setLabel(idShapes, "CIR", contours[i]);
                RotatedRect R=minAreaRect(contours[i]); // Get bounding box for contour i
                R.size.height=R.size.height+R.size.height/3.;
                R.size.width=R.size.width+R.size.width/3.;
                ROIrects.push_back(R);
            }
        }




        //    sort(ROIrects.begin(),ROIrects.end(),&MainWindow::checkcenter);
        std::sort(ROIrects.begin(), ROIrects.end(),  top_to_bottom());       //sort the vector based on height
        int k = 0;
        //    vector<RotatedRect> ROIrectsTemp;
        //    for (std::vector<RotatedRect>::iterator it = ROIrects.begin() ; it != ROIrects.end(); ++it){
        //        cout<<(*it).size.height<<endl;
        //        if (k>0){
        //            if (!(((*--it).center.x)/((*++it).center.x) <1.1 && ((*--it).center.x)/((*++it).center.x) >-1.1)){
        //                ROIrects.erase(ROIrects.begin()+k+1);
        ////                it--;
        //            }
        //        }
        //        k+=1;
        //    }



        drawRectangles(ROIrects,src);  ///sets image for 4th video feed in GUI

        for( int i = 0; i< ROIrects.size(); i++ ){
            Point2f rect_points[4]; ROIrects[i].points( rect_points );
            for( int j = 0; j < 4; j++ )
                line( SrcRoi, rect_points[j], rect_points[(j+1)%4], Scalar(255,0,0), 1, 8 );}
        //    cvtColor(src1,src1,CV_RGB2BGR);

        //    Mat drawing1 = Mat::zeros( SrcRoi.size(), CV_8UC3 );
            Mat drawing2 = Mat::zeros( SrcRoi.size(), CV_8UC3 );


        //    selectROI(drawing1,drawing2);

    //        /// Show in a window
    //        namedWindow( "Contours3", CV_WINDOW_FREERATIO );
    //        imshow( "Contours3", drawing2 );




        //    cv::waitKey(0);
}

void MainWindow:: detectShapes()
{
/*
 *
*/
    //cv::Mat contourImage = cv::imread("polygon.png");
    //    cv::Mat contourImage = cv::imread("c:/scripts/stop.jpg");
    //    cv::GaussianBlur(contourImage,contourImage,cv::Size(5,5),0,0);
    //    cv::threshold(contourImage,contourImage,100,255,0);
    if (cnt_img.empty())
        return ;

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(cnt_img, gray, CV_BGR2GRAY);

    // Use Canny instead of threshold to catch squares with gradient shading
//    cv::Mat bw;
    cv::Canny(gray, gray, 0, 10, 5);

    // Find contours
//    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(gray.clone(), contours, CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);
    RNG rng(12345);
    Mat canny_output;
    //        vector<vector<Point> > con1;
    vector<Vec4i> hierarchy;

    /// Detect edges using canny
    //        Canny( contourImage_gray, canny_output, thresh, thresh*2, 3 );
    /// Find con1
    //        findcon( canny_output, con1, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Draws contours
    shapes = Mat::zeros( gray.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours(shapes, contours, i, color, 2, 8, hierarchy, 0, Point() );
    }



    //give funtion an image of contours, and an image on which to draw. Also can specify an offset in x and y
    return ;
}

