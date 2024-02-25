#include <iostream>
#include <utility>
#include <fstream>
#include <algorithm>
#include <vector>
#include <limits.h>
#include "brain.hpp"
using namespace std;
 

bool Brain::readStations()
{
	cout << "reading stations..." << endl;

    std::ifstream stationsFile("stations.txt");

    if(stationsFile.is_open())
    {
        stationsFile >> stationsCount;

		if(stationsCount < 2)
		{
        	throw "can't create city with " + to_string(stationsCount) + " stations!";
			return 0;
		}
		
		stations = new string[stationsCount];

		int index;
		string name;
		int lineCount = 0, arrayIndex = 0;

		while(stationsFile >> index >> name)
		{
			if(index != arrayIndex)
			{
				throw "index in line " + to_string(lineCount) + " isn't continuable!";
				return 0;
			}
			
			stations[arrayIndex] = name;
			arrayIndex++;
			lineCount++;
		}

		stationsFile.close();

		cout << "found " << stationsCount << " stations." << endl;
		return 1;
    }
	else
	{
        throw "can't open stations.txt";
		return 0;
	}
}

bool Brain::readTSLines()
{
	cout << "reading taxi/subway lines..." << endl;
	std::ifstream taxiAndSubwayLines ("taxi_lines.txt");
	
    if(taxiAndSubwayLines.is_open())
    {
		int taxiLinesCount = 0;	
		int node0, node1, dis;
		int line;

        while(taxiAndSubwayLines >> node0 >> node1 >> dis >> line)
        {
			taxiLinesCount++;

            pair<int,int> key;
            key.first = min(node0, node1);
            key.second = max(node0, node1);
            
            adjencencyMatrix[node0][node1] = 1;
            adjencencyMatrix[node1][node0] = 1;

            if (paths.find(key) == paths.end()) 
            {
                // not found
                Path newPath(subway,dis,line);
                paths[key] = newPath;

            }else 
            {
                // found
				paths[key].setPath(subway, dis, line);
            }
        }

		taxiAndSubwayLines.close();

		cout << "found " + to_string(taxiLinesCount) + " taxi/subway lines." << endl;
		return 1;
	}
	else 
	{
        throw "can't open taxi_lines.txt";
		return 0;
	}
}

bool Brain::readBusLines()
{
	cout << "reading bus lines..." << endl;

    std::ifstream busLines ("bus_lines.txt");

    if(busLines.is_open())
    {
		int busLinesCount = 0;
		int node0, node1, dis;
		int line;

        while(busLines >> node0 >> node1 >> dis >> line)
        {
			busLinesCount++;

            pair<int,int> key;
            key.first = min(node0, node1);
            key.second = max(node0, node1);

            adjencencyMatrix[node0][node1] = 1;
            adjencencyMatrix[node1][node0] = 1;

            if (paths.find(key) == paths.end()) 
            {
                // not found
                Path newPath(bus, dis, line);
                paths[key] = newPath;

            }else 
            {
                // found
                paths[key].setPath(bus, dis, line);
            }
        }

		busLines.close();

		cout << "found " << busLinesCount << " bus lines." << endl;
		return 1;

    }else 
	{
        throw "can't open bus_lines.txt";
		return 0;
	}

}

bool Brain::createCity()
{
	cout << "creating city" << endl;

    //read stations
	try
	{
		readStations();
		//set size of adjencency matrix
		adjencencyMatrix = new bool*[stationsCount];
		for(int i=0; i < stationsCount; i++)
		{
			adjencencyMatrix[i] = new bool[stationsCount];
			for(int j=0; j < stationsCount; j++)
				adjencencyMatrix[i][j] = 0;
		}
	}
	catch(string e)
	{
		cout << "* Error while reading stations:\n\t" << e << endl;
	}

	//read taxi/subway lines
	try
	{
		readTSLines();
	}
	catch(string e)
	{
		cout << "* Error while reading taxi/subway lines:\n\t" << e << endl;
	}

	//read taxi/subway lines
	try
	{
		readBusLines();
	}
	catch(string e)
	{
		cout << "* Error while reading bus lines:\n\t" << e << endl;
	}

	return 1;
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
