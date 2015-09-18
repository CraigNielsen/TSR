#ifndef IMAGEMANIPULATOR_H
#define IMAGEMANIPULATOR_H
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <vector>
using namespace std;
using namespace cv;

class ImageManipulator
{
public:
    ImageManipulator();
    void findShapes(Mat &contourImage, vector<RotatedRect> &ROIrects, Mat &src);
private:
    double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);
    bool checkcenter(const Rect *Rect1, const Rect *Rect2);
    void setLabel(cv::Mat &im, const std::string label, std::vector<cv::Point> &contour);
};

#endif // IMAGEMANIPULATOR_H
