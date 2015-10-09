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
//    namedWindow( "source_window",2);
//    imshow( "source_window", BW );
//    waitKey(0) ;
    cout << topc<< " "<< topr <<" "<< botlc<<" "<<botlr<<" "<< botrc<< " "<< botrr<<endl;

}
//int yLocationLeft(Mat & image)
//{
//    //for each column starting left, count non-zero.
//    //when not == zero, that is the left most column.
//    //search for the top white pixel in that column using the returned count non zero value
//    //iterate through the white pixels in the column until until find white pixel

//    for ( int i =0 ; i < image.cols; i++)
//    {
//        if (countNonZero(image.col(i))>0)
//        {
//            for (int j=0;j<image.rows;j++)
//            {
//                if (image.at<uchar>(j,i)==255)
//                {
//                    imshow("im",image);
//                    waitKey(0);
//                    return j;
//                }
//            }
//        }
//    }
//}
//int leftRowWhiteCount(Mat & image)
//{
//    ///cannot pass image without white pixels

//    //for each column starting left, count non-zero.
//    //when not == zero, that is the left most column.
//    //count all white pixels in that column
//    int x = 0;
//    for ( int i =0 ; i < image.cols; i++)
//    {
//        x=countNonZero(image.col(i));
//        if (x>0)
//        {
//            return x;
//        }
//    }
//}
//int yLocationRight(Mat & image)
//{
//    //for each column starting right, count non-zero.
//    //when not == zero, that is the left most column.
//    //search for the top white pixel in that column using the returned count non zero value
//    //iterate through the white pixels in the column until until find white pixel

//    for ( int i =image.cols-1 ; i >= 0; i--)
//    {

//        if (countNonZero(image.col(i))>0)
//        {
//            for (int j=0;j<image.rows;j++)
//            {
//                if (image.at<uchar>(j,i)==255)
//                {
//                    imshow("im",image);
//                    waitKey(0);
//                    return j;
//                }
//            }
//        }
//    }
//}
bool MainWindow::binaryShape(Mat & src_,string & type)
{
    Mat triangle=imread("/home/craig/Pictures/training_images/shape/tri.png",CV_LOAD_IMAGE_GRAYSCALE);
    triangle=triangle>80;
    Mat circle=imread("/home/craig/Pictures/training_images/shape/circ.png",CV_LOAD_IMAGE_GRAYSCALE);
    src_=src_>80;

    Mat combine=src_.clone();
    Mat combine2=src_.clone();
    Mat combinel=src_.clone();
    Mat combiner=src_.clone();
    Mat cUtry=src_.clone();

    bitwise_and(triangle,src_,combine);
    bitwise_and(circle,src_,combine2);
    bitwise_and(leftBinary,src_,combinel);
    bitwise_and(rightBinary,src_,combiner);
    bitwise_and(utri,src_,cUtry);


    //get count for all non zero pixels

    int r=countNonZero(combine);
    int c=countNonZero(combine2);
    int left=countNonZero(combinel);
    int right=countNonZero(combiner);
//    int totalPercent=countNonZero(src_)*100/(src_.rows*src_.cols);
    int ut=countNonZero(cUtry);


    if ((r+c)< 10 || left ==0 || right==0 /*|| total > 50*/ ){return false;}
//    imshow("tri",src_);
//    waitKey(0);
//    imshow("tri",triangle);
//    waitKey(0);
//    imshow("tri",combine);
//    waitKey(0);
//    imshow("tri",circle);
//    waitKey(0);
//    imshow("circle",combine2);
//    imshow("utrycombined",cUtry);
//    waitKey(0);
    if (r>c)
    {
//    cropTraingle(src_,roii);
        type="tri";
//        cout<<"tri"<<endl;
        return true;
    }
    else if (c>ut){
    type="circle";
//    cout<<"circ"<<endl;
    return true;
    }
    else {
        type="utri";
        return true;
    }
}

string MainWindow::getShape(Mat &src_)
//will return true if triangle or circle.
//false if its just noise
{

//  create a feature vector

    if (src_.channels()>1){cout<<"this is a 3 channel image, please use the 3C function instead"<<endl;}
    imshow("window:",src_);
//    waitKey(1000);
    double ly=trainer.yLocationLeft(src_);
    double ry=trainer.yLocationRight(src_);
    double leftRow=trainer.leftRowWhiteCount(src_);
    double totalPercent=countNonZero(src_)*100/(src_.rows*src_.cols);
//    double first2=trainer.countFirst2(src_);
//    double last2=trainer.countLast2(src_);

    //the cascade is read in with the init function named df3 (df3 defined in header..note : its complicated)
    //return true for triangle, false if its a circle
    // get the feature row of test vector

    if (totalPercent>50 || totalPercent<6){return "notSign";}
    dlib::matrix<double,tImageCols,1>  m;
//    get1DFeatureRow(src_,m);old one

    m(0)=ly;
    m(1)=ry;
    m(2)=leftRow;
    m(3)=totalPercent;
//    m(4)=first2;
//    m(5)=last2;
    // get the label of the test vector
    string label;

    cout << "dlib predicted label: "<< df3(m)<< "for"<< ly << " " <<ry << " " <<leftRow << " " <<totalPercent << " " << endl;
    if (df3(m)==1){label="utri";}
    else if (df3(m)==0){label="circle";}
    else if (df3(m)==2){label="tri";}
    return label;


}
