#include <iostream>
#include <map>
#include "tehran.hpp"
using namespace std;

void Tehran::CreatCity()
{
    map <string,int> v;
    v.insert({"chaharbagh",1});
    v.insert({"kashani",2});
    tehran[1][2].Set_distance(5);
    tehran[1][2].Set_taxi(1);
    tehran[2][1].Set_distance(5);
    tehran[2][1].Set_taxi(1);
    //to be continued
}


