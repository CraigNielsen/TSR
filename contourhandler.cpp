#include "contourhandler.h"

contourHandler::contourHandler()
{
}

void contourHandler::getMassCentres(_Contours & contours1, _Points & mc )
{
    ///________________________CENTERS OF MASS OF CONTOURS______________________________________________
    int cont=contours1.size();
    vector<Moments> mu( cont);
    for( int i = 0; i < cont; i++ )
     { mu[i] = moments( contours1[i], false ); }

     //Insert all 2D points to this vector
    for( int i = 0; i < cont; i++ )
    {

        mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 );
        if ( !((mc[i].x + mc[i].y)>10)  ) {cout<<"delete this contour and mc";}//bool check to see not NAN

    }
}

void contourHandler::removeContourForCentreWithColour(Mat fromImage,_Points & centres,_Contours & contours)
{
    ///removes contours with centre of a specific colour
    /// the colour is set inside the colour setter (still need to make)

    vector<Point2f> pointsForSearch;
//    vector<bool> centreRed(contours.size());
    int cont=contours.size();
    for( int i = 0; i < cont; i++ )
    {
    if ( (centres[i].x + centres[i].y)>10  ) //bool check to see not NAN
    {

        //checks four points around the centre

        Vec3b color1 = fromImage.at<Vec3b>(centres[i].y,centres[i].x);
        Vec3b color2 = fromImage.at<Vec3b>(centres[i].y+2,centres[i].x+2);
        Vec3b color3 = fromImage.at<Vec3b>(centres[i].y-2,centres[i].x-2);
        Vec3b color4 = fromImage.at<Vec3b>(centres[i].y+1,centres[i].x);

        uchar r= color1[2];
        uchar g= color1[1];
        uchar b= color1[0];

        uchar r2= color2[2];
        uchar g2= color2[1];
        uchar b2= color2[0];

        uchar r3= color3[2];
        uchar g3= color3[1];
        uchar b3= color3[0];

        uchar r4= color4[2];
        uchar g4= color4[1];
        uchar b4= color4[0];


        if ( (r > b  && (r-b)>80  && r > g  && (r-g)>80 && r>50 ) ||
             (r2 > b2  && (r2-b2)>80  && r2> g2  && (r2-g2)>80 && r2>50 ) ||
             (r3 > b3  && (r3-b3)>80  && r3 > g3  && (r3-g3)>80 && r3>50 ) ||
             (r4 > b4  && (r4-b4)>80  && r4 > g4  && (r4-g4)>80 && r4>50 )
             )
        {
//            centreRed[i]=true;
            //can delete contour here
//            vec.erase(vec.begin() + index);
            contours.erase(contours.begin()+i);
            centres.erase(centres.begin()+i);

        }
        else
        {
//            centreRed[i]=false;
//            pointsForSearch.push_back(mc[i]);
        }

    }
    }
}


