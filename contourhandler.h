#ifndef CONTOURHANDLER_H
#define CONTOURHANDLER_H
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
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
};

#endif // CONTOURHANDLER_H
