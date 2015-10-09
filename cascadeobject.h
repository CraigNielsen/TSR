#ifndef CASCADEOBJECT_H
#define CASCADEOBJECT_H

#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;


class CascadeObject
{
public:
    CascadeObject();
    CascadeObject(string xmlLocation);
    void setXmlFile(string pathToXml);
    CascadeClassifier classifier;
    void detectSigns(Mat & image);
};

#endif // CASCADEOBJECT_H
