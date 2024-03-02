#include "time_base_navigator.h"
using namespace std;

int TimeBaseNavigator::minNode(DijkstraNode nodes[], bool visited[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < *stationsCount; v++)
		if (visited[v] == false && nodes[v].currentTime.getInMinute() <= min)
			min = nodes[v].currentTime.getInMinute(), min_index = v;

	return min_index;
}


void TimeBaseNavigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = (*paths)[pathKey];

	//calculate subway, taxi and bus duration
	int busTotalDuration = busDelay + (busDuration * path.getBusDis());
	int subwayTotalDuration = subwayDelay + (subwayDuration * path.getSubwayAndTaxiDis());
	int taxiTotalDuration = taxiDelay + (taxiDuration * path.getSubwayAndTaxiDis());
	if(currentNode.vehicles.size())
	{
		Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size()-1];
		if(lastVehicle == bus && path.getBusLine() == currentNode.lastBusLine)
			busTotalDuration -= busDelay;
		else if(lastVehicle == subway && path.getSubwayLine() == currentNode.lastSubwayLine)
			subwayTotalDuration -= subwayDelay;
        else if(lastVehicle == taxi)
            taxiTotalDuration -= taxiDelay;
	}
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
		if(busTotalDuration < subwayTotalDuration)
			vehicle = bus;
		else
			vehicle = subway;
	}
	else
		vehicle = bus;
	
	if(currentNode.currentTime.getTaxiTrafficHour())
	taxiDuration *= 2; 
	if(currentNode.currentTime.getBusSubwayTrafficHour())
	{
		subwayDuration *= 2;
		busDuration *= 3;
	}
	//check if new path is better or not
	if(vehicle == bus)
	{
		if(busTotalDuration + currentNode.currentTime.getInMinute() < nextNode.currentTime.getInMinute())
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useBus(currentNode,path);
		}

	}
	else if(vehicle = subway)
	{
		if(subwayTotalDuration + currentNode.currentTime.getInMinute() < nextNode.currentTime.getInMinute())
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useSubway(currentNode,path);
		}
	}
    else
    {
        if(taxiTotalDuration + currentNode.currentTime.getInMinute() < nextNode.currentTime.getInMinute())
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useTaxi(currentNode,path);
		}
    }
}

DijkstraNode TimeBaseNavigator::navigate(int src, int des)
{
    cout << "starting time base navigator..." << endl;

	DijkstraNode nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
	{
		nodes[i].currentTime.setInfinity();
		visited[i] = false;
	}	

	nodes[src].currentTime.setFromMinute(0);
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
				updateNode(u, v, nodes[u], nodes[v]);
			}
		}
	}

	return nodes[des];
}