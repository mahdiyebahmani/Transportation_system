#include "cost_base_navigator.h"
using namespace std;

int CostBaseNavigator::minNode(DijkstraNode nodes[], bool visited[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < *stationsCount; v++)
		if (visited[v] == false && nodes[v].costUntilNow <= min)
			min = nodes[v].costUntilNow, min_index = v;

	return min_index;
}


void CostBaseNavigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = (*paths)[pathKey];

	//calculate subway, taxi and bus cost
	int busTotalCost = busCost;
	int subwayTotalCost = subwayCost;
	int taxiTotalCost = taxiCost * path.getSubwayAndTaxiDis();
	if(currentNode.vehicles.size())
	{
		Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size()-1];
		if(lastVehicle == bus && path.getBusLine() == currentNode.lastBusLine)
			busTotalCost = 0;
		else if(lastVehicle == subway && path.getSubwayLine() == currentNode.lastSubwayLine)
			subwayTotalCost = 0;
	}
	bool busAvailable = path.getBusDis();
	bool STAvailable = path.getSubwayAndTaxiDis();
	
	//between bus path and subway/taxi path select min
	Vehicle vehicle;
	if(busAvailable && STAvailable)
	{
		if(busTotalCost < subwayTotalCost)
		{
			if(busTotalCost < taxiTotalCost)
				vehicle = bus;
			else
				vehicle = taxi;
		}
		else
		{
			if(subwayTotalCost < taxiTotalCost)
				vehicle = subway;
			else
				vehicle = taxi;
		}
			
	}
	else if(STAvailable)
	{
		if(busTotalCost < subwayTotalCost)
			vehicle = bus;
		else
			vehicle = subway;
	}
	else
		vehicle = bus;

	if(currentNode.currentTime.getTaxiTrafficHour())
	taxiTotalCost *= 1.5;
	if(currentNode.currentTime.getBusSubwayTrafficHour())
	{
		busTotalCost *= 2;
		subwayTotalCost *= 3;
	}

	//check if new path is better or not
	if(vehicle == bus)
	{
		if(busTotalCost + currentNode.costUntilNow < nextNode.costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useBus(currentNode,path);
		}

	}
	else if(vehicle == subway)
	{
		if(subwayTotalCost + currentNode.costUntilNow < nextNode.costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useSubway(currentNode,path);
		}
	}
	else
	{
		if(taxiTotalCost + currentNode.costUntilNow < nextNode.costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNode = Navigator::useTaxi(currentNode,path);
		}
	}
}

DijkstraNode CostBaseNavigator::navigate(int src, int des)
{
    cout << "starting cost base navigator..." << endl;

	DijkstraNode nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
		nodes[i].costUntilNow = INT_MAX, visited[i] = false;

	// Distance of source vertex from itself is always 0
	nodes[src].costUntilNow = 0;
	nodes[src].paths.push_back(src);

	// Find shortest path for all vertices
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