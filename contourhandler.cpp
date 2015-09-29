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

    _Points pointsForSearch;
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


            if ( (r > b  && (r-b)>80  && r > g  && (r-g)>80 ) ||
                 (r2 > b2  && (r2-b2)>80  && r2> g2  && (r2-g2)>80 ) ||
                 (r3 > b3  && (r3-b3)>80  && r3 > g3  && (r3-g3)>80 ) ||
                 (r4 > b4  && (r4-b4)>80  && r4 > g4  && (r4-g4)>80  )
                 )
            {
                //            centreRed[i]=true;
                //can delete contour here
                //            vec.erase(vec.begin() + index);
                contours.erase(contours.begin()+i);
                centres.erase(centres.begin()+i);

            }
            ///CHECK AREA HERE


        }
    }
}

void contourHandler::removeContourswithArea(_Contours &contours, int area)
{
    ///iterate through the contours
    /// check area of min area rect arounf the contour
    /// remove contours  with an area larger than given area
    for (_Contours::iterator it=contours.begin();it!=contours.end();)
    {
        /// a little trick
        /// checks are of item in vector
        /// deletes item using erase which also returns a pointer to the next item
        /// http://bojan-komazec.blogspot.co.uk/2011/07/remove-selected-vector-elements-whilst.html
        RotatedRect r=minAreaRect(*it);
        float areaQ= r.size.area();
        if (areaQ>area)
        {
            it=contours.erase(it);
        }
        else{++it;}
    }
}
void addToTempMap(float cpoint,float addpoint)
{

}
void contourHandler::getCloseContours(_Points &centresOfMass, int pointDistance, map <int,_Points> & mep )
{
    ///search____________________________________________________________________________
    //iterate through all centres of mass. and querie closest points to each point
    //  add a group to a vector if more than 1 NN
    // search vector group to see if current point exists in group. (skip if yes, search if no)
    vector<int> indices;
    vector<float> dists;
    int pointName=-1;
    pointDistance=pointDistance*pointDistance;

    if (centresOfMass.size()>1)
    {
        for (_Points::iterator it = centresOfMass.begin();it!=centresOfMass.end();it++)

        {
            ///iterate through all points in centres of mass collection
            /// for each point find all the nearest neighbours
            ///  add all nearest neighbours with distanve requirements to a map with the point name as a key (pointname is point name)
            ///the indices is the indicy to reference the centreofMass ALWAYS
            Point c=*it;
            pointName+=1;
            int numOfPoints=centresOfMass.size();
            flann::KDTreeIndexParams indexParams;
            flann::Index kdtree(Mat(centresOfMass).reshape(1), indexParams);
            vector<float> query;
            query.push_back(c.x); //Insert the 2D point we need to find neighbours to the query
            query.push_back(c.y); //Insert the 2D point we need to find neighbours to the query
            kdtree.knnSearch(query,indices,dists,numOfPoints);
            //all nearest are in centresofMass with indices.

            //        kdtree.radiusSearch(query, indices, dists, range, numOfPoints);
            for (vector<int>::iterator it2 = indices.begin();it2!=indices.end();it2++)
            {
                int what=*it2;
                if (dists[what]<pointDistance && dists[what]>1)
                {
                    mep[pointName].push_back(centresOfMass[indices[what]]);
                }
            }
        }
    }
}
void contourHandler::drawCloseContours(_Points &centresOfMass, _Contours contours,map<int,vector<Point2f>> & closemap, Mat & image)
{
    ///for all points associated with the centreofMass index (which is the name of closeContour in map) draw lines between them
    /// for each centre of mass, draw lines between the points of the closest contours on the given image
    Scalar colour=Scalar( 255, 255, 255 );
    int thickness=5;

    for (unsigned int i = 0 ; i<centresOfMass.size();i++)
    {
        //get the vector associated with the current centre of mass
        _Points points=closemap[i];
        if (points.size()>2)
        {
            //for a point, if the close points are more than one:

            Point2f start=centresOfMass[i];
            Point2f stop=closemap[i].back();
            //draw lines between the points in the vector
            //draw from first to last point once
            line(image,start,stop,colour,thickness);
            //then all points in succession
            for (_Points::iterator point=points.begin();point!=points.end();point)
            {
                Point2f start=*point;
                Point2f stop=*(point++);
                line(image,start,stop,colour,thickness);
            }
        }
        else if (points.size()==2)
        {
            Point2f start=centresOfMass[i];
            Point2f stop=closemap[i].back();
            //draw lines between the points in the vector
            //draw from first to last point once
            line(image,start,stop,colour,thickness);
        }

    }
}

void contourHandler::getContourRoi(_Contours &contours)
{

}




