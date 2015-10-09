#include "cascadeobject.h"

CascadeObject::CascadeObject()
{
    cout<<"please pass in a xml location"<<endl;
}
CascadeObject::CascadeObject(string xmlLocation)
{
    classifier.load(xmlLocation);
}

void CascadeObject::setXmlFile(string pathToXml)
{
    classifier.load(pathToXml);
}

void CascadeObject::detectSigns(Mat &image)
{
    // Detect signs
    std::vector<Rect> signs;
    classifier.detectMultiScale( image, signs, 1.1, 5, 0|CV_HAAR_SCALE_IMAGE, Size(40, 40), Size(120, 120) );

    // Draw circles on the detected signs
    for( int i = 0; i < signs.size(); i++ )
    {
        rectangle(image,signs[i],Scalar(255,255,255),1);
        Point center( signs[i].x + signs[i].width*0.5, signs[i].y + signs[i].height*0.5 );
        ellipse( image, center, Size( signs[i].width*0.5, signs[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
    }
    namedWindow("Detected Sign",2);
    imshow( "Detected Sign", image );

//    waitKey(10);
}
