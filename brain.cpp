#include <iostream>
#include <utility>
#include <fstream>
#include "brain.hpp"
using namespace std;
 
void Brain::createCity()
{
    //read station count
    std::ifstream stationsFile("stations.txt");
    if(stationsFile.is_open())
    {
        stationsFile >> stationsCount;
    }
    stationsFile.close();

    adjencencyMatrix = new bool*[stationsCount];
    for(int i=0; i < stationsCount; i++)
       adjencencyMatrix[i] = new bool[stationsCount];
    
    //read taxi and subway lines
    std::ifstream taxiAndSubwayLines ("taxi_lines.txt");

    int node0, node1, dis;
    int subwayLine;

    if(taxiAndSubwayLines.is_open())
    {
        while(taxiAndSubwayLines >> node0 >> node1 >> dis >> subwayLine)
        {
            pair<int,int> key;
            pair.first = Min(node0, node1);
            pair.second = Max(node0, node1);
            
            adjencencyMatrix[node0][node1] = 1;
            adjencencyMatrix[node1][node0] = 1;

            if (stations.find(key) == stations.end()) 
            {
                // not found
                Path newPath(dis,subwayLine);
                stations[key] = newPath;

            }else 
            {
                // found
                Path path = stations[key];
                if(path.subway_and_taxi == 0)
                {
                    path.set_subway_and_taxi(true);
                    path.set_subway_and_taxi_dis(dis);
                    path.set_subway_line(subwayLine);
                }
            }
        }
    }else 
        cout << "Error occured while opening taxi_lines.txt"<<endl;

    taxiAndSubwayLines.close();

    //read bus lines
    std::ifstream busLines ("bus_lines.txt");
    if(busLines.is_open())
    {
        while(busLines >> node0 >> node1 >> dis)
        {
            pair<int,int> key;
            pair.first = Min(node0, node1);
            pair.second = Max(node0, node1);

            adjencencyMatrix[node0][node1] = 1;
            adjencencyMatrix[node1][node0] = 1;

            if (stations.find(key) == stations.end()) 
            {
                // not found
                Path newPath(0,dis);
                stations[key] = newPath;

            }else 
            {
                // found
                Path path = stations[key];
                if(path.bus == 0)
                {
                    path.set_bus(true);
                    path.set_bus_dis(dis);
                }
            }
        }
    }else 
        cout << "Error occured while opening bus_lines.txt"<<endl;

    busLines.close();
}