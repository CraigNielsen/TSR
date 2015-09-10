#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <iostream>
using namespace std;


void MainWindow::cropTraingle(Mat & BW,Mat &roii)
{
    //look for points in bw image

    int c = BW.cols/2;
    int topr=c;
    int topc=c;
    int botlr=c;
    int botlc=c;
    int botrr=c;
    int botrc=c;

    for (int i=0 ; i< BW.rows;i++) {
        uchar * pix = BW.ptr<uchar>(i);
        for (int j=0; j<BW.cols;j++){
            int p=(int)pix[j];
           if (p==255) {
               if (i<topr){topr=i;topc=j;}
               if (i>botlr){botlr=i;}
               if (j<botlc){botlc=j;}
               if (i>botrr){botrr=i;}
               if (j>botrc){botrc=j;}
           }
        }
    }

    // scale points up to ROI size
    float s=roii.rows/BW.rows;
    //need to have a global white image of size 50x50
    // define triangle and extract with affine  into white image
    Point2f srcTri[3];
    Point2f dstTri[3];
    Mat rot_mat( 2, 3, CV_32FC1 ); //lets see what affine does first
    Mat warp_mat( 2, 3, CV_32FC1 );
    Mat warp_dst;

    warp_dst = Mat::zeros( roii.rows, roii.cols, roii.type() );

    /// Set your 3 points to calculate the  Affine Transform
    srcTri[0] = Point2f( (s*topc),(s*topr));
    srcTri[1] = Point2f( (s*botlc),(s*botlr ));
    srcTri[2] = Point2f( (s*botrc), (s*botrr) );


    dstTri[0] = Point2f( roii.cols/2 , 0  );
    dstTri[1] = Point2f( 0, roii.rows-1 );
    dstTri[2] = Point2f( roii.cols-1,roii.rows-1);
    ///draw line on roi
    line( roii, srcTri[0], srcTri[1], Scalar( 0, 0, 0 ), 1);
    line( roii, srcTri[1], srcTri[2], Scalar( 0, 0, 0 ), 1 );
    line( roii, srcTri[2], srcTri[0], Scalar( 0, 0, 0 ), 1 );
    /// Get the Affine Transform
    warp_mat = getAffineTransform( srcTri, dstTri );
    /// Apply the Affine Transform just found to the src image
    warpAffine(roii, warp_dst, warp_mat, warp_dst.size() );

    /// Show what you got
    namedWindow( "source_window",2);
    imshow( "source_window", BW );
    waitKey(0) ;
    cout << topc<< " "<< topr <<" "<< botlc<<" "<<botlr<<" "<< botrc<< " "<< botrr<<endl;

}
void MainWindow::getShape(Mat &src_,Mat & roii)
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
    cropTraingle(src_,roii);}
    else {cout << "circle" << endl;}
    if (src_.channels()>1){cout<<"this is a 3 channel image, please use the 3C function instead"<<endl;return ;}


//    dlib::matrix<double,tImageCols,1>  m;
//    get1DFeatureRow(src_,m);

//    // get the label of the test vecto

//    for (int i =0 ; i< m.size() ; i++){

//    }
//    cout << "predicted label: "<< df3(m)<< endl;
//    samples.pop_back();



}
