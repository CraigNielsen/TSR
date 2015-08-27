#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;

//image – Source, an 8-bit single-channel image.
//Non-zero pixels are treated as 1’s. Zero pixels remain 0’s,
//so the image is treated as binary . You can use compare() ,
//inRange() , threshold() , adaptiveThreshold() , Canny() ,
//and others to create a binary image out of a grayscale or
//color one. The function modifies the image while extracting
//the contours.
//http://opencvexamples.blogspot.com/2013/09/find-contour.html (Source)


void MainWindow::findMyContours()
{

/*Instantiates image_contourTemp from img_morphex
 * if the checkbox is ticked, it inverts the image
 * find contours in img_ContourTemp
 * draws contours on cnt_image
 * dispays cnt_image in gui
*/
    img_morphex.copyTo(img_ContourTemp);
    if(ui->checkBox->checkState()){
        invertGrayImage(img_ContourTemp);
    }

    vector<Vec4i> hierarchy;                                                    //for contour hierarchy needed for different levels (contours within contours)
    findContours(img_ContourTemp,
                 contours, // a vector of contours
                 hierarchy,
                 CV_RETR_CCOMP, // retrieve the external contours
                 CV_CHAIN_APPROX_SIMPLE); // all pixels of each contour


    drawContours( cnt_img, contours, -1 , contourColor,
                  1, CV_AA, hierarchy,4  );
    idShapes = cnt_img.clone();
    final=cnt_img.clone();
    //Draw Contours Parameters:

    //    image – Destination image.
    //    contours – All the input contours. Each contour is stored as a point vector.
    //    contourIdx – Parameter indicating a contour to draw. If it is negative, all the contours are drawn.
    //    color – Color of the contours.
    //    thickness – Thickness of lines the contours are drawn with. If it is negative (for example, thickness=CV_FILLED ), the contour interiors are drawn.
    //    lineType – Line connectivity. See line() for details.
    //    hierarchy – Optional information about hierarchy. It is only needed if you want to draw only some of the contours (see maxLevel ).
    //    maxLevel – Maximal level for drawn contours. If it is 0, only the specified contour is drawn. If it is 1, the function draws the contour(s) and all the nested contours. If it is 2, the function draws the contours, all the nested contours, all the nested-to-nested contours, and so on. This parameter is only taken into account when there is hierarchy available.
    //    offset – Optional contour shift parameter. Shift all the drawn contours by the specified \texttt{offset}=(dx,dy) .
    //    contour – Pointer to the first contour.
    //    externalColor – Color of external contours.
    //    holeColor – Color of internal contours (holes).


    //set image to label
    //blur contour image for another process
//            GaussianBlur( temp, src_gray, Size(GaussBlurAmt,GaussBlurAmt), 0, 0, BORDER_DEFAULT );}




//    QPixmap map;
//    QImage ProPic1((uchar*)(cnt_img).data,(cnt_img).cols,(cnt_img).rows,(cnt_img).step,QImage::Format_RGB888);
//    map.convertFromImage(ProPic1);
//    ui->contourDisplayImg->setPixmap(map);//end of wf 1



///__________________________________________________________________________
  ///___________________END OF WORKFLOW ONE__________________________________



}

void MainWindow::findRectangles()
{

    contours2.clear();
    //    auto contours(contours2);
    contours2.insert(contours2.end(), contours.begin(), contours.end());
    //        Start to iterate to each contour found

    vector<vector<Point> >::iterator itc= contours2.begin();
    rects.clear();

    Mat rec_img = Mat::zeros(final.rows, final.cols, CV_8UC3);
    //Remove patch that has no inside limits of aspect ratio and area.
    while (itc!=contours2.end()) {
        //Create bounding rect of object
        RotatedRect mr= minAreaRect(Mat(*itc));
        if( !checkSizes(mr)){
            itc= contours2.erase(itc);
        }else{
            ++itc;
            rects.push_back(mr);
        }
    }
    cout<<rects.size()<<endl;
    for (RotatedRect i : rects){
        cout<<i.size<<endl;
    }
    drawRectangles(rects,rec_img);



    //    Point2f vertices[4];
    //    for (int i = 0; i < 4; i++){
    //        rects[0].points(vertices);

    //        line(rec_img, vertices[i], vertices[(i+1)%4], Scalar(0,255,0));}
    //    QPixmap map3;
    //    QImage ProPic2((uchar*)(rec_img).data,(rec_img).cols,(rec_img).rows,(rec_img).step,QImage::Format_RGB888);
    //    map3.convertFromImage(ProPic2);
    //    ui->rectangleDisplayImg->setPixmap(map3);

}


void MainWindow::drawRectangles(vector<RotatedRect> rects, Mat &image)
{
    for( int i = 0; i< contours2.size(); i++ )
       {
         Scalar color = Scalar( 0, 0, 255 );

         // rotated rectangle
         Point2f rect_points[4];
         rects[i].points( rect_points );
         for( int j = 0; j < 4; j++ )
            line( image, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
       }
    //set image to label
    QPixmap map2;
    QImage ProPic2((uchar*)(image).data,(image).cols,(image).rows,(image).step,QImage::Format_RGB888);
    map2.convertFromImage(ProPic2);
    ui->rectangleDisplayImg->setPixmap(map2);


}
void MainWindow::drawRectangle(RotatedRect rect,Mat &image){
    Scalar color2 = Scalar( 0, 0, 255 );

    // rotated rectangle
    Point2f rect_points[4];
    rect.points( rect_points );
    for( int j = 0; j < 4; j++ )
       line( image, rect_points[j], rect_points[(j+1)%4], color2, 4, 8 );

}
bool MainWindow::checkSizes(RotatedRect candidate ){

    //Spain car plate size: 52x11 aspect 4,7272


    //Set a min and max area. All other patches are discarded

    int min= (1)  *heightOfRectangleSqed; // minimum area 2
    int max= (10)    *heightOfRectangleSqed; // maximum area 5
                                                                                ///Width Height aspect ratio:

    float rmin= 1-2 *TolleranceError;//min ratio  (ONE HERE MEANS sQUARE)
    float rmax= 5+  5   *TolleranceError;//max ratio
    ///INSERT A PIECE OF CODE TO CHECK FOR SQAURES.. ACCEPT MORE                NEED TO EXPORT ALL RECTANGLES FOR FURTHER ANALYSIS
    int area= candidate.size.height * candidate.size.width;

    ui->minAreaLabel->setText(QString::number(min));
    ui->maxAreaLabel->setText(QString::number(max));
    ui->minRatioLabel->setText(QString::number(rmin));
    ui->maxRatioLabel->setText(QString::number(rmax));

    float angle =candidate.angle;
    if (candidate.size.width<candidate.size.height){
        angle+=90;
    }
    if (angle<0){angle+=360;}
    if (angle>10 && angle<350 ){return false;}
    float r= (float)candidate.size.width / (float)candidate.size.height;
    if(r<1)                                                                     //measn we will pick up upright rectangles too
        r= 1/r;
    if(( area < min || area > max ) || ( r < rmin || r > rmax )){
        return false;
    }else{
        cout<<"INFOMATION STARTS HERE"<<"\n"<<
               candidate.size.height<<"\n"
            << candidate.size.width<<"\n"
            << candidate.size.area()<<"\n________"
            << angle
            << endl;
        return true;
    }
}

//==================================GUI  =======================================


void MainWindow::on_contourRModeSlider_valueChanged(int value)
{
    contourModeValue=value;
    switch (value){
    case 0:ui->contourRetrievalModeLabel->setText("CV_RETR_EXTERNAL");
        ui->contourRetrievalModeLabel->setToolTip("retrieves only the extreme outer contours.");break;
    case 1:ui->contourRetrievalModeLabel->setText("CV_RETR_LIST");
        ui->contourRetrievalModeLabel->setToolTip("retrieves all of the contours without establishing any hierarchical relationships.");break;
    case 2:ui->contourRetrievalModeLabel->setText("CV_RETR_CCOMP");
        ui->contourRetrievalModeLabel->setToolTip("retrieves all of the contours "
         "and organizes them into a two-level hierarchy. At the top level, there"
         " are external boundaries of the components. At the second level, there"
         " are boundaries of the holes. If there is another contour inside a hole"
         " of a connected component, it is still put at the top level.");break;
    case 3:ui->contourRetrievalModeLabel->setText("CV_RETR_TREE");
        ui->contourRetrievalModeLabel->setToolTip("retrieves all of the contours and reconstructs a full hierarchy of nested contours. ");break;
    }
    findMyContours();
}
