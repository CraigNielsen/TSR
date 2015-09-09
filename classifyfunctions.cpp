#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;


void MainWindow::cropTraingle(Mat & BW,Mat &roi)
{
    //look for points in bw image
    int topr;
    int topc;
    int c = BW.cols/2;
    int botlr=c;
    int botlc=c;
    int botrr=c;
    int botrc=c;
    bool topdone;
    for (int i=0 ; i< BW.rows;i++) {
        uchar * pix = BW.ptr<uchar>(i);
        for (int j=0; j<BW.cols;j++){
            int p=(int)pix[j];
           if (p==255) {
               if (!topdone){ topr=i;topc=j;topdone=true;}
               if (i>botlr){botlr=i;}
               if (j<botlc){botlc=j;}
               if (i>botrr){botrr=i;}
               if (j>botrc){botrc=j;}
           }
        }
    }
    cout << topc<< " "<< topr <<" "<< botlc<<" "<<botlr<<" "<< botrc<< " "<< botrr<<endl;
    // scale points up to ROI size
    //need to have a global white image of size 50x50
    // define triangle and extract with affine  into white image

}
void MainWindow::getShape(Mat &src_,Mat & roi)
{
    //the cascade is read in with the init function named df3 (df3 defined in header..note : its complicated)

    // get the feature row of test vector
    Mat triangle=imread("/home/craig/Pictures/training_images/shape/tri.png",CV_LOAD_IMAGE_GRAYSCALE);
    triangle=triangle>80;

    Mat circle=imread("/home/craig/Pictures/training_images/shape/circ.png",CV_LOAD_IMAGE_GRAYSCALE);

    src_=src_>80;
    Mat combine=src_.clone();
    Mat combine2=src_.clone();
    bitwise_and(triangle,src_,combine);
    bitwise_and(circle,src_,combine2);
    int r=countNonZero(combine);
    int c=countNonZero(combine2);
    if (r>c){cout<<"triangle"<<endl;
    cropTraingle(src_,roi);}
    else {cout << "circle" << endl;}
    if (src_.channels()>1){cout<<"this is a 3 channel image, please use the 3C function instead"<<endl;return ;}
    dlib::matrix<double,tImageCols,1>  m;
    get1DFeatureRow(src_,m);

    // get the label of the test vector
//    cout<<samples.size()<<endl;
    for (int i =0 ; i< m.size() ; i++){
//        cout<<"out is " << samples[0](i) << endl;
    }
    cout << "predicted label: "<< df3(m)<< endl;
    samples.pop_back();

//    namedWindow("roi",2);
//    imshow("roi",src_);
//    waitKey(0);


}
