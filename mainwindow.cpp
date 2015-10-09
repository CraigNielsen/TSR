#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
using namespace std;


#define IMAGE_DIR "/home/craig/QT/scripts/"

//#define VIDEO_PATH "/home/craig/Videos/50_1080p_Test.dvd"
//#define VIDEO_PATH "/home/craig/Videos/long.mp4"
//#define VIDEO_PATH "/home/craig/Videos/30_720p.mp4"
//#define VIDEO_PATH "/home/craig/Videos/Svizzera_AlexFreeStockVideo.mp4"
//#define VIDEO_PATH "/home/craig/Videos/debug1.dvd"
#define VIDEO_PATH "/media/craig/Memory 2/Videos/TestVideo1.mp4"
//#define VIDEO_PATH "/media/craig/Memory 2/Videos/1.avi"


void MainWindow::setInitVariables()
{

    //Gui valuesm

    roiPath="/home/craig/Pictures/training_images/ROI/";
    craigbug=false;
    writeROI=false;
    writeROIbw=false;
    showdetections=true;
    bitwise_shape=true;
    size_= cv::Size(11,11);
    name=1;
    pointDistance = 100;
    leftBinary=imread("/home/craig/Pictures/training_images/shape/left.png",CV_LOAD_IMAGE_GRAYSCALE);
    rightBinary=imread("/home/craig/Pictures/training_images/shape/right.png",CV_LOAD_IMAGE_GRAYSCALE);
    utri=imread("/home/craig/Pictures/training_images/shape/utri.png",CV_LOAD_IMAGE_GRAYSCALE);
    triangleSign=imread("/home/craig/Pictures/test_images/tri.png",CV_LOAD_IMAGE_COLOR);
    circleSign=imread("/home/craig/Pictures/test_images/circ.jpg",CV_LOAD_IMAGE_COLOR);
    utriSign=imread("/home/craig/Pictures/test_images/utri.png",CV_LOAD_IMAGE_COLOR);

    if(!triangleSign.data){(cout<<"no trianglesign"<<endl);}
    if(!circleSign.data){(cout<<"no circlesign");}

    cout<<"tri channels "<<triangleSign.rows<< endl;
    cout<<"circle rows:  "<<circleSign.rows<< endl;
    Size s=Size(50,50);
    cv::resize(triangleSign,triangleSign,s,0,0,INTER_NEAREST);
    cv::resize(circleSign,circleSign,s,0,0,INTER_NEAREST);
    cv::resize(utriSign,utriSign,s,0,0,INTER_NEAREST);
    huetop=180;
    huebottom=170;

    //_____________ CASCADE DETECTOR LOAD _______________


//    cascade.setXmlFile("/home/craig/git_repos/CascadeTrainingOpenCV/data/cascade.xml");
//    utriCascade.setXmlFile("/home/craig/git_repos/TSR/utriCascade.xml");
//    stopSignCascade.setXmlFile("/home/craig/git_repos/TSR/stopSignCascade.xml");
//    triangleCascade.setXmlFile("/home/craig/git_repos/TSR/triangleCascade.xml");
//    triangleCascade.setXmlFile("/home/craig/git_repos/TSR/triCas30_2.xml");
    triangleCascade.setXmlFile("/home/craig/git_repos/TSR/all30_cascase.xml");
    //_____________LOAD  DLIB CLASSFIER _________________

//    svm.load("/home/craig/scripts/road1.xml"); // loading
//    // load the function back in from disk and store it in df3.
    try {
    dlib::deserialize("df.dat") >> df3;
    cout<<"DLIB .dat loaded";
    }
    catch (exception)
    {
        cout<<"is the .dat file for classifier dlib in the built directory?";
    }

//    featureRowTemp_=Mat(1,tImageCols,CV_8UC1);
    //__________________ROI HEIGHT FOR SOURCE IMAGE__________________________________
    int topx=srcCols/10;
    int topy=srcRows/4;
    int width=srcCols-2*topx;
    int height=srcRows/3;
    //__________________________________________________
    //TEST VIDEOS:
//    int topx=srcCols/10;
//    int topy=0;
//    int width=srcCols-2*topx;
//    int height=srcRows;
    //__________________________________________________
    roi = cv::Rect(topx,topy,width,height);
    a=0;b=0;c=62;d=255;e=46;f=255;
    minA=200;
    maxA=15000;
    ui->kernalWidthSpin->setValue(3);
    ui->kernalTypeSpin->setValue(0);
    ui->morphOperatorSlider->setValue(1);
    ui->GBlurAmtSlider->setSingleStep(3);
    ui->kernal_width_slider->setValue(3);
    ui->kernal_height_slider->setValue(3);
    ui->contourRModeSlider->setValue(1);

    //____________________________SObel_________________________________________
    GaussBlurAmt = 7;
    SobelKernalSize =3;
    gradient=0.45;
    scale = 1;
    delta = 0;
    ddepth = CV_16S;
//    //________________________MorphologyEx____________________________________
    kernalType = 0;
    morph_size = 0;
    morph_width = 5;
    morph_height = 5;
    morph_operator = 1;
    updateSizeLabel();
    updateKernalTypeLabel();
    updateMorphologyTypeLabel();
    //__________________________________________________________________________
    //_______________________Contours and Rectangles____________________________
    contourModeValue=0;
    heightOfRectangleSqed=5;
    TolleranceError=1/10.;


}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_open_clicked()  //__________________________________________Open Image/refresh Image
{
    cv::VideoCapture capture(VIDEO_PATH);
    srcCols=capture.get(CV_CAP_PROP_FRAME_WIDTH);
    srcRows=capture.get(CV_CAP_PROP_FRAME_HEIGHT);
    double rate= capture.get(CV_CAP_PROP_FPS);
    cv::Mat frame;

//    string name=IMAGE_DIR+(ui->nameSpin->text()).toStdString()+".jpg";
//    src=imread(name);
//    if (!src.data){return ;}
//    srcCopy2=src.clone();

//    lowHeightSquared=lowHeightSquared*src.rows*src.rows;
//    highHeightSquared=highHeightSquared*src.rows*src.rows;
//    cout<<"__________________"<<lowHeightSquared<<"   \n"<<highHeightSquared<<endl;


    setInitVariables();
    cout<<"done here"<<endl;
    bool stop = false;

    while (!stop) {                                                             // for all frames in video
        frameNo +=1;
        if (ui->df->isChecked())
        {
            for (int i = 0; i < 20; i++)
            {
                    capture.read(frame);
            }
        }
        if (! ui->pause_button->isChecked())
        {
            if (!capture.read(frame)){break;} ;
        }

//        change.brightness(frame,0);
        //______________________________________________________________________________PART 1
        Setup(frame);
        //Threshold NEEDS WORK
        benallallRGB();
        //try converted to HSV space
//        hsvSpace();
        //try a cascade classifier
//        triangleCascade.detectSigns(frame);

        //BLUR
        if (ui->cb_Gauss->isChecked()){
            GaussianBlur( src_gray, src_gray, Size(GaussBlurAmt,GaussBlurAmt), 0, 0, BORDER_DEFAULT );}
        //EDGES
        XYEdges();

//        imBWUpdate();                                          //displays image in window SOBEL

        updateMorphImage();                                                         //display Morphex image

        findMyContours();   // FIND CONTOURS AND DRAWS CONTOURS


        detectShapes();

//        detectROIShapes_one();        //extra process to experimaent with regions of interest
        //__________________________________________________________________________PART 2
        Mat drawing = Mat::zeros( SrcRoi.size(), CV_8UC3 );

        vector<vector<Point> > contours1;

        getContours(contours1);//operates on image: final
        vector<Rect> recAreas( contours1.size() );
        db("1");
        if (contours1.size()>0){
            db(1.1);
            selectROI(contours1,recAreas);
            db(1.2);
            classifyShape(contours1,recAreas,SrcRoi);
        }
        db(2);
            /// Show in a window
//            namedWindow( "Contours", CV_WINDOW_FREERATIO );
//            imshow( "Contours", drawing );



        imUpdate(ui->ol,&img_threshold);
        imUpdate(ui->contourDisplayImg,&cnt_img,"colour");
//        imUpdate(ui->rectangleDisplayImg,&src);


///        Show in a window
//        namedWindow("test",2);
//        imshow("test",src_gray);

//        namedWindow("Source Clean",CV_WINDOW_FREERATIO);
//        cv::imshow("Source Clean", src_clean);

        namedWindow( "Contours2", 2 );
        imshow( "Contours2", SrcRoi );
        imUpdate(ui->finalvideo,&SrcRoi,"colour");

        cout<<"Frame NUmber is: "<<frameNo<<endl;
//        namedWindow("processed",CV_WINDOW_FREERATIO);
//        cv::imshow("processed",framout);

        cv::waitKey(timeout);                                                                               // or press key to stop
        if (cv::waitKey(1)>=0)
            stop= true;
    }


}
void MainWindow::showImage(Mat &image,char*name,string flags)
{
    if (flags=="size"){
        string str1=QString::number(image.cols).toStdString()+" cols x "+QString::number(image.rows).toStdString()+" rows";
        putText(image,str1,Point(0,image.rows-1),0,1,Scalar(0,0,0),2);
    }

    char * string = name;
    namedWindow(string,CV_WINDOW_FREERATIO);
    imshow(string,image);

}



void MainWindow::invertGrayImage(Mat &image)
{
    image=1-image;
}

void MainWindow::setupWindow2()
{
    cout<<"so far so good"<<endl;


}








void MainWindow::imUpdate(QLabel * imageDisplay, Mat * CVimage, String colour)                 //imUpdate   accepts Qlabel pointer and CV image pointer and updates the label with the image.
{
    // image converted from BGR to RGB
//    cv::cvtColor(*CVimage,*CVimage,CV_BGR2RGB);


    if (colour=="colour"){

        cvtColor(*CVimage,*CVimage,CV_BGR2RGB);
        QImage ProPic((uchar*)(*CVimage).data,(*CVimage).cols,(*CVimage).rows,(*CVimage).step,QImage::Format_RGB888);
        QPixmap map;
        map.convertFromImage(ProPic);
        imageDisplay->setPixmap(map);
    }
    else {

        QImage ProPic((uchar*)(*CVimage).data,(*CVimage).cols,(*CVimage).rows,(*CVimage).step,QImage::Format_Indexed8);
        QPixmap map;
        map.convertFromImage(ProPic);
        imageDisplay->setPixmap(map);}


}



void MainWindow::on_processRectanglesButton_clicked()
{
    findRectangles();
}

void MainWindow::on_rectHeightSlider_valueChanged(int value)
{
    heightOfRectangleSqed=value*value;
    ui->aspectRatioLabel->setText("rectangle height: "+ QString::number(value));
    findRectangles();
}

void MainWindow::on_rectangleToleranceSlider_valueChanged(int value)
{
    TolleranceError=value/10.;
    ui->toleranceLabel->setText(QString::number(value/10.));
    findRectangles();
}

void MainWindow::on_drawItOnOriginal_clicked()
{

    src.copyTo(srcCopy1);
    findRectangles();
    img_roiMask.release();                                                      //release memory from mask so new mask can be found and image can be refreshed using bool (image.data) in floodfill process
//    drawRectangles(rects,srcCopy1);
    floodfillProcess();                                         //info:
    cropping();

//    setupWindow2(); ////can delete this eventually



}



void MainWindow::on_checkBox_clicked()
{
    invertGrayImage(img_ContourTemp);
    findMyContours();
}



void MainWindow::on_finalThresholdSlider_2_valueChanged(int value)
{
//    ui->finalThresholdLabel->setText(QString::number(value));
//    thresh_Low=value;
//    Mat grayResult;
//    cvtColor(croppedImageResultResized, grayResult, CV_BGR2GRAY);
//    equalizeHist(grayResult, grayResult);
//    blur(grayResult, grayResult, Size(3,3));

//    bitwise_not ( grayResult, grayResult );

//    threshold(grayResult,grayResult,thresh_Low,thresh_High,0);

////        invertGrayImage(grayResult);

//    showImage(grayResult,"final cropped image");
    timeout=value;
}

void MainWindow::on_nameSpin_valueChanged(int arg1)
{
    try
     {
       string name="/home/craig/scripts/"+(ui->nameSpin->text()).toStdString()+".jpg";
       img_previewImport=imread(name);
       imUpdate(ui->previewLabel,&img_previewImport,"colour");
     }
     catch (exception& e)
     {
//       cout << e.what() << '\n';
       ui->previewLabel->setText("Doesnt exist");
       return;
     }


}

void MainWindow::on_sb_a_valueChanged(int arg1)
{
    a=arg1;
}


void MainWindow::on_sb_b_valueChanged(int arg1)
{
    b=arg1;
}
void MainWindow::on_sb_c_valueChanged(int arg1)
{
    c=arg1;
}


void MainWindow::on_sb_d_valueChanged(int arg1)
{
    d=arg1;
}
void MainWindow::on_sb_e_valueChanged(int arg1)
{
    e=arg1;
}


void MainWindow::on_sb_f_valueChanged(int arg1)
{
    f=arg1;
}

void MainWindow::on_minA_valueChanged(int arg1)
{
    minA=arg1;
}

void MainWindow::on_maxA_valueChanged(int arg1)
{
    maxA=arg1;
}



void MainWindow::on_verticalSlider_valueChanged(int value)
{
    pointDistance=value;
}

void MainWindow::on_hueslidertop_valueChanged(int value)
{
    huetop=value;
}

void MainWindow::on_huesliderbottom_valueChanged(int value)
{
    huebottom=value;
}

void MainWindow::on_exportROIbw_clicked()
{

    writeROIbw=!writeROIbw;
}

void MainWindow::on_verticalSlider_2_valueChanged(int value)
{
    sat=value;
    ui->S->setText(QString(value));
}

void MainWindow::on_verticalSlider_3_valueChanged(int _value)
{
    value=_value;
    ui->V->setText(QString(_value));
}

void MainWindow::on_showdetects_clicked()
{
    showdetections=!showdetections;
}
