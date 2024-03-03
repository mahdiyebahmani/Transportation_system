#include "time_base_navigator.h"
using namespace std;

int TimeBaseNavigator::minNode(DijkstraNode nodes[], bool visited[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < *stationsCount; v++)
		if (visited[v] == false && nodes[v].currentTimeInMinute <= min)
			min = nodes[v].currentTimeInMinute, min_index = v;

	return min_index;
}


void TimeBaseNavigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode, Clock startTime)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = (*paths)[pathKey];

	
	//check traffic time for subway
	int subwayNewDelay = subwayDelay;
	startTime.addMinute(currentNode.currentTimeInMinute);
	if(startTime.isBusSubwayTrafficHour())
	{
		subwayNewDelay *= 3;
	}
	//calculate subway, taxi and bus duration
	int busTotalDuration = busDelay + (busDuration * path.getBusDis());
	int subwayTotalDuration = subwayNewDelay + (subwayDuration * path.getSubwayAndTaxiDis());
	int taxiTotalDuration = taxiDelay + (taxiDuration * path.getSubwayAndTaxiDis());
	if(currentNode.vehicles.size())
	{
		Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size()-1];
		if(lastVehicle == bus && path.getBusLine() == currentNode.lastBusLine)
			busTotalDuration -= busDelay;
		else if(lastVehicle == subway && path.getSubwayLine() == currentNode.lastSubwayLine)
			subwayTotalDuration -= subwayNewDelay;
        else if(lastVehicle == taxi)
            taxiTotalDuration -= taxiDelay;
	}

	//check traffic time for others
	if(startTime.isBusSubwayTrafficHour())
		busTotalDuration *= 2;
	if(startTime.isTaxiTrafficHour())
		taxiTotalDuration *= 2;


	bool busAvailable = path.getBusDis();
	bool STAvailable = path.getSubwayAndTaxiDis();
	//between bus path and subway/taxi path select min
	Vehicle vehicle;
	if(busAvailable && STAvailable)
	{
		if(busTotalDuration < subwayTotalDuration)
		{
			if(busTotalDuration < taxiTotalDuration)
				vehicle = bus;
			else
				vehicle = taxi;
		}
		else
		{
			if(subwayTotalDuration < taxiTotalDuration)
				vehicle = subway;
			else
				vehicle = taxi;
		}
			
	}
	else if(STAvailable)
	{
		if(taxiTotalDuration < subwayTotalDuration)
			vehicle = taxi;
		else
			vehicle = subway;
	}
	else
		vehicle = bus;


	//check if new path is better or not
	if(vehicle == bus)
	{
		if(busTotalDuration + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useBus(currentNode,path, startTime);
		}

	}
	else if(vehicle == subway)
	{
		if(subwayTotalDuration + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useSubway(currentNode,path, startTime);
		}
	}
    else
    {
        if(taxiTotalDuration + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useTaxi(currentNode,path, startTime);
		}
    }
}

DijkstraNode  TimeBaseNavigator::navigate(int src, int des, Clock startTime)
{
    cout << "starting time base navigator..." << endl;

	DijkstraNode nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
		nodes[i].currentTimeInMinute = INT_MAX, visited[i] = false;


	nodes[src].currentTimeInMinute = 0;
	nodes[src].paths.push_back(src);


	for (int count = 0; count < *stationsCount; count++) {

		// Find the lowest between nodes that are not visited
		int u = minNode(nodes, visited);
		// Mark the picked vertex as processed
		visited[u] = true;

		// Update node value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < *stationsCount; v++)
		{
			// Update node[v]
			if (adjencencyMatrix[u][v] && !visited[v])
			{
				updateNode(u, v, nodes[u], nodes[v], startTime);
			}
		}
	}

	return nodes[des];
}