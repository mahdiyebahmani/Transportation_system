#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include <map>
#include "path.hpp"
using namespace std;

class Brain
{
    public:
    
    void createCity();


    int minDistance(int[] ,bool[]);

    void dijkstra(int **, int );

    int return_distance(int ,int);
    
    private:
    int stationsCount = 59;
    map< pair<int,int>, Path > stations;
    bool **adjencencyMatrix;

};

#endif