#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits.h>
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
            key.first = min(node0, node1);
            key.second = max(node0, node1);
            
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
                if(path.get_subway_and_taxi() == 0)
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
            key.first = min(node0, node1);
            key.second = max(node0, node1);

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
                if(path.get_bus() == 0)
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

int Brain::minDistance(D_node dist[], bool sptSet[])
{

	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < stationsCount; v++)
		if (sptSet[v] == false && dist[v].dis <= min)
			min = dist[v].dis, min_index = v;

	return min_index;
}


void Brain::dijkstra(int **graph, int src)
{
	D_node dist[stationsCount]; // The output array. dist[i] will hold the
				// shortest
	// distance from src to i

	bool sptSet[stationsCount]; // sptSet[i] will be true if vertex i is
					// included in shortest
	// path tree or shortest distance from src to i is
	// finalized

	// Initialize all distances as INFINITE and stpSet[] as
	// false
	for (int i = 0; i < stationsCount; i++)
		dist[i].dis = INT_MAX, sptSet[i] = false;

	// Distance of source vertex from itself is always 0
	dist[src].dis = 0;

	
	// Find shortest path for all vertices
	for (int count = 0; count < stationsCount - 1; count++) {
		// Pick the minimum distance vertex from the set of
		// vertices not yet processed. u is always equal to
		// src in the first iteration.
		int u = minDistance(dist, sptSet);

		// Mark the picked vertex as processed
		sptSet[u] = true;
        dist->visited.push_back(u);
		// Update dist value of the adjacent vertices of the
		// picked vertex.
		for (int v = 0; v < stationsCount; v++)

			// Update dist[v] only if is not in sptSet,
			// there is an edge from u to v, and total
			// weight of path from src to v through u is
			// smaller than current value of dist[v]

			if (!sptSet[v] && graph[u][v] == 1 && dist[u].dis != INT_MAX && dist[u].dis + return_distance(u,v) < dist[v].dis)
                dist[v].dis = dist[u].dis + return_distance(u,v);
				
	}

}


int Brain::return_distance(int u , int v)
{
    pair<int,int>key;
    key.first = u;
    key.second = v; 

    if (key.first > key.second )
    {
        if (stations[{key.first,key.second}].get_subway_and_taxi())
        return stations[{key.first,key.second}].get_subway_and_taxi_dis();
    }
       
    else
        return stations[{key.second,key.first}].get_bus_dis();

}
