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


struct DijkstraNode
{
    DijkstraNode(): costUntilNow{0}{}
    int disToSource;
    int currentTimeInMinute;
    int costUntilNow;
    std::vector<int> paths;
    std::vector<Vehicle> vehicles;
    int lastSubwayLine;
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

    int stationsCount;
    std::string* stations;
    std::map< std::pair<int,int>, Path > paths;
    bool **adjencencyMatrix;

};

#endif