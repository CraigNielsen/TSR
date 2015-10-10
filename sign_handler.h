#ifndef SIGN_HANDLER_H
#define SIGN_HANDLER_H
#include <opencv2/core/core.hpp>
#include <map>
#include <string>
#include <vector>
#include <iostream>

//#define print(a) cout<<a<<endl;
//#define print(a,b,c) cout<<a<<" "<<b<<" "<<c<<endl;
//#define print(a,b) cout<<a<<" "<<b<<endl;
using namespace std;
typedef map<int,cv::Point> loc;
typedef loc::iterator locit;
typedef map<int,int>::iterator frameit;
typedef map<int,vector<string>>::iterator labelit;
class sign_handler
{
public:
    sign_handler();
    map<int,vector<string>> labels;
    map<int,cv::Point> locations;
    map<int,int> frame;
    int frameRedundancy=9;
        //add new shape to the object.
    void add(cv::Point p, string label, int frameNo);//check not already a point, add if not "inArea()",otherwise update current location and  add label
    map<int,string> checklabels();
    //object can search its current items and compare based on x,y coordinates


private:
    int name=1;
    int inArea(cv::Point & p); //return the value of the key for same location
    void checkframe(int frameNo);

    //use position to find last shape in same position.

    //if object has 5 same shapes, classify(bring up image)
    //object can contain areas for possible shapes
    //object can contain each areas past shape classifications..
    //object can handle showing sign on screen

    //every time a position is checked, the object will update position and give a new label.
    //object will remove possibles if 10 frames pass without close detection
};

#endif // SIGN_HANDLER_H
