#ifndef CITY
#define CITY

#include <iostream>
#include <utility>
#include <fstream>
#include <climits>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "path.hpp"
#include "clock.h"
#include "dis_base_navigator.h"
#include "cost_base_navigator.h"


class City
{
    public:
    City();
    bool createCity();
    std::string findBestPath(std::string, std::string, Clock);

    private:
    bool readStations();
    bool readTSLines();
    bool readBusLines();

    std::string getPathDataAsString(DijkstraNode, Clock);

    int stationsCount;
    std::string* stations;
    std::map< std::pair<int,int>, Path > paths;
    bool **adjencencyMatrix;

    DisBaseNavigator disNavigator;
    CostBaseNavigator costBaseNavigator;
};

#endif