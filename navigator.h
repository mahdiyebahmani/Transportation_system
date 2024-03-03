#ifndef NAVIGATOR
#define NAVIGATOR

#include <iostream>
#include <climits>
#include <map>
#include <vector>
#include <algorithm>
#include "path.hpp"
#include "clock.h"

class Navigator
{
    public:
    void init(int* ,std::map< std::pair<int,int>, Path >*, bool**);
    DijkstraNode navigate(int, int, Clock);

    protected:
    int minNode(DijkstraNode*, bool*);
    DijkstraNode useBus(DijkstraNode, Path, Clock);
    DijkstraNode useTaxi(DijkstraNode, Path, Clock);
    DijkstraNode useSubway(DijkstraNode, Path, Clock);
    void updateNode(int&, int&, DijkstraNode, DijkstraNode&, Clock);

    int* stationsCount;
    std::map< std::pair<int,int>, Path >* paths;
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