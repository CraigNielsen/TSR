#ifndef CONTOURHANDLER_H
#define CONTOURHANDLER_H
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <map>
#include <opencv2/flann/flann.hpp>
using namespace cv;
using namespace std;


typedef vector<vector<Point> > _Contours;//Contour
typedef vector<Point2f>  _Points;//Mass centre

class contourHandler
{
public:
    contourHandler();
    void getMassCentres(_Contours & contours1, _Points &mc);
    void removeContourForCentreWithColour(Mat fromImage, _Points &centres, _Contours &contours);
    void removeContourswithArea(_Contours & contours, int area);
    void getCloseContours(_Points & centresOfMass, int pointDistance, map<int, _Points> &mep);
    void getContourRoi(_Contours & contours);
    void drawCloseContours(_Points & centresOfMass, _Contours contours, map<int, vector<Point2f> > &closemap, Mat &image);
};

#endif // CONTOURHANDLER_H
