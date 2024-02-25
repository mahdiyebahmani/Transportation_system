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
    DijkstraNode dijkstra(int, int);

    private:
    
    bool readStations();
    bool readTSLines();
    bool readBusLines();
    
    int minDistance(DijkstraNode*, bool*);
    DijkstraNode useBus(DijkstraNode, Path);
    DijkstraNode useTaxi(DijkstraNode, Path);
    DijkstraNode useSubway(DijkstraNode, Path);
    void updateNode(int&, int&, DijkstraNode, DijkstraNode&);

    int stationsCount;
    std::string* stations;
    std::map< std::pair<int,int>, Path > paths;
    bool **adjencencyMatrix;

    int busCost = 2250;
    int subwayCost = 3267;
    int taxiCost = 6000;

    int busDuration = 4;
    int subwayDuration = 1;
    int taxiDuration = 2;

    int busDelay = 15;
    int subwayDelay = 8;
    int taxiDelay = 5;

};

#endif