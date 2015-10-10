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

void CascadeObject::detectSigns(Mat &image, string type,string file)
{
    // Detect signs
    std::vector<Rect> signs;
    classifier.detectMultiScale( image, signs, 1.3, 2, 0|CV_HAAR_SCALE_IMAGE, Size(20, 20), Size(60, 60) );
    int fontface = cv::FONT_HERSHEY_SIMPLEX;
    double scale = 2;
    int thickness = 3;
    string input;
    string boolean;
    // Draw circles on the detected signs
    for( int i = 0; i < signs.size(); i++ )
    {
        rectangle(image,signs[i],Scalar(255,255,255),1);
        Point center( signs[i].x + signs[i].width*0.5, signs[i].y + signs[i].height*0.5 );
        ellipse( image, center, Size( signs[i].width*0.5, signs[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
        cv::putText(image,(type),Point (signs[i].x ,signs[i].y), fontface, scale,
                    CV_RGB(0,0,255), thickness, 8);
        string locationX=file.substr(38,46) + ".txt";
        ofstream myfile (locationX , fstream::in | fstream::out | fstream::app);

        cout << "Please enter true or false value: t or f ";
        cin >> input;
        cout << "you entered " << input << endl;
        if (input == "t")  boolean="all true" ;
        if (input == "y")  boolean="wrong shape true region";
        else boolean="false";
        if (myfile.is_open())
        {
          myfile <<"x: "<< signs[i].x <<" y: "<< signs[i].y << " type: "<< type << " "<< boolean << endl;
          myfile << "___________________\n";
          myfile.close();
        }
        else cout << "Unable to open file";
    }


    namedWindow("Detected Sign",2);
    imshow( "Detected Sign", image );

    waitKey(10);
}
