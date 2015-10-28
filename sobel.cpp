#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;

//     /*SOBEL function takes the following arguments: EDGE DETECTION
//                                                                              src_gray: In our example, the input image. Here it is CV_8U
//                                                                              grad_x/grad_y: The output image.
//                                                                              ddepth: The depth of the output image. We set it to CV_16S to avoid overflow.
//                                                                              x_order: The order of the derivative in x direction.
//                                                                              y_order: The order of the derivative in y direction.
//                                                                              scale, delta and BORDER_DEFAULT: We use default values.*/
void MainWindow::RGB_Iterator(Mat& frame, const uchar* const table)
{





}
void MainWindow::getRed_inRGB(Mat &src)
{
/*Can Optimize here if want to by not converting from 3 channels to one, just start with 1 channel image
 * had troubles so left it to come back to, expense is speed
*/


    int r,g,b;
    int delRG=50;
    int delRB=50;
//    Mat blank=Mat::zeros(SrcRoi.rows,SrcRoi.cols,CV_8UC3);
//    Mat output = SrcRoi.clone();
    for (int i=0; i<src.rows ; i++)
    {
        Vec3b* opixel = src.ptr<Vec3b>(i);

        for (int j=0 ; j<src.cols; j++)
        {
            //RGB values for a pixel
            r= opixel[j][2];
            g= opixel[j][1];
            b= opixel[j][0];

            if ( (r > b  && (r-b)>delRB  && r > g  && (r-g)>delRG  ) )
            {
                //RGB values for a pixel
                opixel[j][2]=255;
                opixel[j][1]=255;
                opixel[j][0]=255;
            }
            else
            {
                //RGB values for a pixel
                opixel[j][2]=0;
                opixel[j][1]=0;
                opixel[j][0]=0;
            }
        }
    }



}
void MainWindow::hsvSpace()
{
    //converts SrcRoi to HSV space
    //tries to find the roi in that space instead of rgb

    Mat hsv=SrcRoi_clean.clone();
    Mat dst=Mat::zeros(SrcRoi.rows,SrcRoi.cols,CV_8UC1);
    cvtColor(hsv,hsv,CV_BGR2HSV);
//    huebottom=160;
//    huetop=180;
    inRange(hsv, Scalar(huebottom, sat, value), Scalar(huetop, 255, 255), dst);
    Mat element = getStructuringElement( kernalType, Size( morph_width, morph_height )/*, Point( morph_size, morph_size )*/ );
    cv::morphologyEx(dst,dst,MORPH_OPEN,element,Point(-1,-1),2);

    cv::morphologyEx(dst,dst,MORPH_CLOSE,element,Point(-1,-1),2);

    namedWindow("hsv",2);
    imshow("hsv",dst);

//    Mat blank=Mat::zeros(SrcRoi.rows,SrcRoi.cols,CV_8UC3);
//    //    Mat output = SrcRoi.clone();
//    for (int i=0; i<SrcRoi.rows-2 ; i+=2)
//    {
//        Vec3b* opixel = blank.ptr<Vec3b>(i);
//        Vec3b* pixel = SrcRoi.ptr<Vec3b>(i);

//        for (int j=0 ; j<SrcRoi.cols-2 ; j+=2)
//        {
//            r= pixel[j][2];
//            g= pixel[j][1];
//            b= pixel[j][0];

//            if ( (r > b  && (r-b)>delRB  && r > g  && (r-g)>delRG  ) )
//            {
//                opixel[j][2]=255;
//                opixel[j][1]=255;
//                opixel[j][0]=255;
//            }
//        }
//    }

//    //    src_gray= blank.clone();
//    cv::inRange(blank, cv::Scalar(20, 20, 20), cv::Scalar(255, 255, 255), src_gray);
}

void MainWindow::benallallRGB()
{
/*iterates through SrcRoi and finds red pixels in RGB space
 * Can Optimize here if want to by not converting from 3 channels to one, just start with 1 channel image
 * had troubles so left it to come back to, expense is speed
*/

    int r,g,b;
    int delRG=60;
    int delRB=60;
    Mat blank=Mat::zeros(SrcRoi.rows,SrcRoi.cols,CV_8UC3);
//    Mat output = SrcRoi.clone();
    for (int i=0; i<SrcRoi.rows-2 ; i+=2)
    {
        Vec3b* opixel = blank.ptr<Vec3b>(i);
        Vec3b* pixel = SrcRoi.ptr<Vec3b>(i);

        for (int j=0 ; j<SrcRoi.cols-2 ; j+=2)
        {
            r= pixel[j][2];
            g= pixel[j][1];
            b= pixel[j][0];

            if ( (r > b  && (r-b)>delRB  && r > g  && (r-g)>delRG  ) )
            {
                opixel[j][2]=255;
                opixel[j][1]=255;
                opixel[j][0]=255;
            }
        }
    }

//    src_gray= blank.clone();
    cv::inRange(blank, cv::Scalar(20, 20, 20), cv::Scalar(255, 255, 255), src_gray);
    imshow("rgb color",src_gray);

}

void MainWindow::Setup(Mat &frame)
{

/*
 * This function takes a src frame from video / image
 * creates the src ,  SrcRoi , cnt_img
*/
    src=frame.clone();
    src_clean=frame.clone();
    SrcRoi=(src(roi));
    SrcRoi_clean=(src(roi)).clone();
    cnt_img = Mat::zeros(SrcRoi.rows, SrcRoi.cols, CV_8UC3);

}

void MainWindow::SobelProcess()
{



    cvtColor( src, src_gray, CV_RGB2GRAY );                          // Convert it to gray
    GaussianBlur( src_gray, src_gray, Size(GaussBlurAmt,GaussBlurAmt), 0, 0, BORDER_DEFAULT );
    Sobel( src_gray, grad_x, ddepth, 1, 0, SobelKernalSize, scale, delta, BORDER_DEFAULT );   // Gradient X
    Sobel( src_gray, grad_y, ddepth, 0, 1, SobelKernalSize, scale, delta, BORDER_DEFAULT );   // Gradient Y
    convertScaleAbs( grad_x, abs_grad_x );                                      //Converts input array elements to an 8-bit unsigned integer array with optional linear transformation
    convertScaleAbs( grad_y, abs_grad_y );                                      // converts to CV_8U
//    displayWeighted();

}

void MainWindow::XYEdges()
{

/*this function.. If sobel is checked, the src_gray is edge processed and thresholded into abs_grad_x and abs_grad_y
 * then combines gradient images, using global gradient saving to img_sobel
 * if sobel is checked, img_sobel is then thresholded and passed to img_threshold
 * otherwise: src_gray is thresholded and passed to img_threshold
*/
    if (ui->cb_sobel->isChecked()){
        Sobel( src_gray, grad_x, ddepth, 1, 0, SobelKernalSize, scale, delta, BORDER_DEFAULT );   // Gradient in X
        Sobel( src_gray, grad_y, ddepth, 0, 1, SobelKernalSize, scale, delta, BORDER_DEFAULT );   // Gradient in Y
        convertScaleAbs( grad_x, abs_grad_x );                                      //Converts input array elements to an 8-bit unsigned integer array with optional linear transformation
        convertScaleAbs( grad_y, abs_grad_y );                                      // converts to CV_8U
        addWeighted( abs_grad_x, gradient, abs_grad_y, 1-gradient, 0, img_sobel ); // Total Gradient (approximate)


}
}



//========================================== GUI STUFF =========================

void MainWindow::imBWUpdate(QLabel * imageDisplay, Mat * CVimage)               //8 bit unsigned//imUpdate   accepts Qlabel pointer and CV image pointer and updates the label with the image.
{
    QImage ProPic((uchar*)(*CVimage).data,(*CVimage).cols,(*CVimage).rows,(*CVimage).step,QImage::Format_Indexed8);
    QPixmap map;
    map.convertFromImage(ProPic);
    imageDisplay->setPixmap(map);
}


void MainWindow::on_GBlurAmtSlider_valueChanged(int value)
{
    value= value*2+1;
    GaussBlurAmt=value;
    ui->GBlurAmtLabel->setText(QString::number(value) + " x " + QString::number(value));
    SobelProcess();
}

void MainWindow::on_sobelKernalSizeSlider_valueChanged(int value)
{
    value= value*2+1;
    SobelKernalSize=value;
    ui->sobelKernalSizeSpinbox->setValue(value);
    switch (value){
    case (1):ui->sobelKernalSizeLabel->setText(QString::number(1)+ " x " + QString::number(1));   break;
    case (3):ui->sobelKernalSizeLabel->setText(QString::number(3)+ " x " + QString::number(3));   break;
    case (5):ui->sobelKernalSizeLabel->setText(QString::number(5)+ " x " + QString::number(5));   break;
    case (7):ui->sobelKernalSizeLabel->setText(QString::number(7)+ " x " + QString::number(7));   break;
    default:break;
    }
    SobelProcess();
}
void MainWindow::on_sobelGradientSlider_valueChanged(int value)
{
    gradient=value/90.;      // to give a value between 0 and 1 for gradient ratio
    ui->sobelGradientSpinBox->setValue(value);
    ui->sobelGradientLabel->setText(QString::number(value)+" Degrees");
    SobelProcess();
}
