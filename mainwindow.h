#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <QLabel>
#include "imagemanipulator.h"
#include "trainerobject.h"
#include "sign_handler.h"
#include "contourhandler.h"
#include "cascadeobject.h"
#include <dlib/svm_threaded.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <dlib/rand.h>
#include <ctime>


#define db(a) if(craigbug){cout<<a<<endl;}

using namespace cv;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{


    Q_OBJECT

    public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    ///__________________________________________
//    char* source="/home/craig/data/image.jpg";
    ///__________________________________________
    /// +++++++++++++++++ CASCADE DETECTION ++++++++++++++++++++++++

    CascadeObject allCascade;
    CascadeObject utriCascade;
    CascadeObject stopSignCascade;
    CascadeObject triangleCascade;
    CascadeObject circleCascade;

    //+++++++++++++___ DLIB  CLASSIFIER ___+++++++++++++++++++++++++++++++++
    static const int tImageCols=4;
    typedef dlib::matrix<double,tImageCols,1> sample_type;
    typedef dlib::one_vs_one_trainer<dlib::any_trainer<sample_type> > ovo_trainer;
    typedef dlib::polynomial_kernel<sample_type> poly_kernel;
    typedef dlib::radial_basis_kernel<sample_type> rbf_kernel;
    dlib::one_vs_one_decision_function<ovo_trainer,
    dlib::decision_function<poly_kernel>,  // This is the output of the poly_trainer
    dlib::decision_function<rbf_kernel>    // This is the output of the rbf_trainer
    > df3;
    void convertToDlib(Mat & src_);
    Mat featureRowTemp_;
    string getShape(Mat & src_);
    void get1DFeatureRow(Mat & img_mat, dlib::matrix<double, tImageCols, 1> & m);
    std::vector<sample_type> samples;
    void printOutMatrix(Mat & in_);
    void cropTraingle(Mat &BW, Mat & roii);
    sign_handler signs;
    int frameNo=0;
    //________________________________CONTROL BOOLS______________________________________
    bool craigbug=false;
    bool writeROI=true;
    bool writeROIbw=true;
    bool bitwise_shape=true;
    bool showdetections=true;
    int pointDistance;
    int countRegions=0;
    string roiPath;
    cv::Size size_;
    bool preProcessROI(Mat &src);
    void selectROI(vector<vector<Point> > & contours1, vector<Rect> &minRect);
    void getContours(vector<vector<Point> > &contours1);
    void classifyShape(vector<vector<Point> > & contours1, vector<Rect> minRect, Mat &frame);
    contourHandler cHandler;
    bool testcorners(Mat & temp);
    //____________________________Global________________________________________
    Mat src,src_cROI, srcCopy1,srcCopy2, img_extractFromHere,img_seedPoints,img_roiMask,img_previewImport,final,src_clean,SrcRoi,SrcRoi_clean;
    int srcRows,srcCols,minA,maxA;
    int name=0;
    Mat triangleSign,circleSign, utriSign;
    Mat rightBinary, leftBinary,utri;
    trainerObject trainer=trainerObject();
    cv::Rect roi;
    float lowHeightSquared=1/10.;
    float highHeightSquared=9/10.;
    Scalar_<double> contourColor = Scalar(255,255,128);
    void showImage(Mat & image, char *name, std::string flags="false");
    ImageManipulator im = ImageManipulator();
    void setInitVariables();
    void invertGrayImage(Mat &image);
    void setupWindow2();
    void hsvSpace();
    int huetop,huebottom,sat,value;
    bool binaryShape(Mat & src_, string &type);
    QPixmap sobelPixelMap;
    int GaussBlurAmt = 3;
    int SobelKernalSize =3;
    Mat src_gray,srcSobel;
    double gradient=0.45;
    Mat img_sobel;                                                              // Output From Sobel
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;
    Mat img_threshold;                                                          // Output From Sobel with threshold applied
    //________________________MorphologyEx_____________________________________
    Mat img_morphex;                                                            //output from morphology process
    int kernalType = 0;
    int morph_size = 0;
    double morph_width = 1;
    double morph_height = 1;
    int morph_operator = 1;
    //__________________________________________________________________________
    //_______________________Contours and Rectangles____________________________
    Mat img_ContourTemp,cnt_img;
    Mat shapes,idShapes;
    int contourModeValue=3;
    vector<vector<Point> > contours;
    void drawRectangles(vector<RotatedRect> rects, Mat &image);
    void drawRectangle(RotatedRect rect, Mat &image);
    int a,b,c,d,e,f;
    float heightOfRectangleSqed=5;                                                             //tollerance aspect ratio of rectangles in image
    float TolleranceError=1/10.;                                                //tollerance for area allowed
    vector<vector<Point> > contours2;
    vector<RotatedRect> rects;
    vector<RotatedRect> ROIrects;
    QImage ProPic;
    void detectROIShapes_one();
    void detectShapes();
    int timeout=1;
    void getRed_inRGB(Mat &src);
    void benallallRGB();
private slots:
    void RGB_Iterator(Mat & frame, const uchar * const table);
    //_________________________GUI SIGNALS_____________________________________
    void on_open_clicked();
    void on_morphOperatorSlider_valueChanged(int value);
    void on_kernal_Type_slider_valueChanged(int value);
    void on_kernal_width_slider_valueChanged(int value);
    void on_MorphologyOperatorDisplay_valueChanged(int arg1);
    void on_kernalTypeSpin_valueChanged(int arg1);
    void on_kernalWidthSpin_valueChanged(int arg1);
    void on_kernal_height_slider_valueChanged(int value);
    void on_kernalHeightSpin_valueChanged(int arg1);
    void on_GBlurAmtSlider_valueChanged(int value);
    void on_sobelKernalSizeSlider_valueChanged(int value);
    void on_sobelGradientSlider_valueChanged(int value);
    void on_contourRModeSlider_valueChanged(int value);
    void on_processRectanglesButton_clicked();
    void on_rectHeightSlider_valueChanged(int value);
    void on_rectangleToleranceSlider_valueChanged(int value);
    void on_drawItOnOriginal_clicked();
    void on_checkBox_clicked();
    void on_finalThresholdSlider_2_valueChanged(int value);
    void on_nameSpin_valueChanged(int arg1);
    void on_sb_a_valueChanged(int arg1);
    void on_sb_b_valueChanged(int arg1);
    void on_sb_c_valueChanged(int arg1);
    void on_sb_d_valueChanged(int arg1);
    void on_sb_e_valueChanged(int arg1);
    void on_sb_f_valueChanged(int arg1);
    void on_minA_valueChanged(int arg1);
    void on_maxA_valueChanged(int arg1);
    void on_verticalSlider_valueChanged(int value);
    void on_hueslidertop_valueChanged(int value);
    void on_huesliderbottom_valueChanged(int value);
    void on_exportROIbw_clicked();
    void on_verticalSlider_2_valueChanged(int value);
    void on_verticalSlider_3_valueChanged(int _value);
    void on_showdetects_clicked();
    void on_pauseOnSHapeButton_clicked();

private:
    Ui::MainWindow *ui;
    //_________________________Sobel____________________________________________
    void SobelProcess();
    void Setup(Mat &frame) ;                               //updates combinedGradient as final product
    void XYEdges();
    void imBWUpdate(QLabel *imageDisplay, Mat *CVimage);
    bool pauseOnShape=false;
    //________________________MorphologyEx______________________________________
    void imUpdate(QLabel *imageDisplay , Mat *CVimage, String colour="gray");
    void updateMorphImage();
    void updateSizeLabel();
    void updateKernalTypeLabel();
    void updateMorphologyTypeLabel();
//_______________________Contours and Rectangles________________________________
    void findRectangles();
    bool checkSizes(RotatedRect candidate);
    void findMyContours();
//____________________________Floodfill/ Extract Regions_________________________________________
    void floodfillProcess();
    void cropping();
    int thresh_Low=230;
    int thresh_High=255;
    Mat croppedImageResultResized;
    void finalCrop(Mat &image);
    bool checkcenter(const Rect *Rect1, const Rect *Rect2);
    void joinCloseRegions(Mat &src_);
};



#endif // MAINWINDOW_H
