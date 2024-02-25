#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include <utility>
#include <fstream>
#include <climits>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include "path.hpp"
using namespace std;


enum v {Bus,taxi_subway};

struct D_node
{ 
    //int base;
    int dis;
    int cost;
    int time;
    vector<int>visited;
    vector<v>vehicle;
};

class Brain
{
    public:
    
    bool createCity();


    int minDistance(D_node [] ,bool[]);

    void dijkstra(int **, int );

    int return_distance(int ,int);
    
    private:

    bool readStations();
    bool readTSLines();
    bool readBusLines();

    int stationsCount = 59;
    map< pair<int,int>, Path > stations;
    bool **adjencencyMatrix;

};

#endif