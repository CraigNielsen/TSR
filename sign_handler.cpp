#include "sign_handler.h"

#define deleteAfter 4

sign_handler::sign_handler()
{
    cout<<"created a sign handler"<<endl;
}

void sign_handler::add(cv::Point p, string label,int frameNo)
{
    int closeto=inArea(p);
    if (closeto==0)
    {
        signCounter++;
        cout<<"sign counter regions: "<<signCounter<<endl;
        labels[name].push_back(label);
        locations[name]=p;
        frame[name]=frameNo;
        name++;
    }
    else
    {
        labels[closeto].push_back(label);
        //update the position
        locations[closeto]=p;
        frame[closeto]=frameNo;
    }
    //check the frames
    checkframe(frameNo);


}

int sign_handler::inArea(cv::Point &p)
{
    for (locit i=locations.begin(); i!=locations.end();i++)
    {
        //check distances for all keys, if within threshold, update point, add label
        if (abs(locations[i->first].x-p.x)<200 && abs(locations[i->first].y-p.y)<200)
        {
            return i->first;
        }
    }
    return 0;
}

void sign_handler::checkframe(int frameNo)
{
    for (frameit i = frame.begin();i!=frame.end();i++)
    {

        if ((frameNo - i->second) > deleteAfter)
        {
            int deletekey=i->first;
            locations.erase(deletekey);
            labels.erase(deletekey);
            frame.erase(i);//deleted using iterator might cause issues here
        }
    }
}

map<int,string> sign_handler::checklabels()
{
    //checks for position with more than 5 labels and tally up the label that wins
    //create a  map for labels etc.
    map<int,string> signs;
    //search for labelobject with more than 5 labels
    for ( labelit i = labels.begin();i!= labels.end();i++)
    {
        int key=i->first;
        vector<string> vs=i->second;

        //count values in vectors with size bigger than 5
        if(vs.size()>frameRedundancy)
        {
            int tricount=0;
            int circount=0;
            int utryCount=0;
            vector<string>::iterator j;
            //count them all
            for (j = vs.begin();j!=vs.end();j++)
            {
                (*j=="tri") ? (tricount+=1) : 0 ;
                (*j=="circle") ? (circount+=1) : 0 ;
                (*j=="utri") ? (utryCount+=1) : 0 ;
            }
            //add key and label to map
            string label;
            if (tricount>circount && tricount > utryCount)  (label="triangle") ;
            else if (circount>tricount && circount > utryCount) (label="circle") ;
            else if (utryCount>circount && utryCount > tricount) (label="utri") ;
            signs[key]=label;
        }
    }
    //return a map with all the keys and the label
    return signs;
}

