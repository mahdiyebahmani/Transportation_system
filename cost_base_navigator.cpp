#include "cost_base_navigator.h"
using namespace std;

int CostBaseNavigator::minNode(vector<DijkstraNode> nodes[], bool visited[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < *stationsCount; v++)
		if (visited[v] == false && nodes[v][0].costUntilNow <= min)
			min = nodes[v][0].costUntilNow, min_index = v;

	return min_index;
}


void CostBaseNavigator::updateNode(int& u, int& v, vector<DijkstraNode> currentNodes, vector<DijkstraNode>& nextNodes, Clock startTime)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = (*paths)[pathKey];

	DijkstraNode& currentNode = currentNodes[0];
	for(int i{0}; i < currentNodes.size(); i++)
	{
		if(currentNodes[i].vehicles.size())
		{
			Vehicle lastVehicle = currentNodes[i].vehicles[currentNode.vehicles.size()-1];
			if(lastVehicle == subway)
				if(currentNodes[i].lastSubwayLine == path.getSubwayLine())
					currentNode = currentNodes[i];
			else if(lastVehicle == bus)
				if(currentNodes[i].lastBusLine == path.getBusLine())
					currentNode = currentNodes[i];
		}
	}
	
	//calculate subway, taxi and bus cost
	int busTotalCost = busCost;
	int subwayTotalCost = subwayCost;
	int taxiTotalCost;
	startTime.addMinute(currentNode.currentTimeInMinute);
	if(startTime.isTaxiTrafficHour())
		taxiTotalCost = path.getSubwayAndTaxiDis() *  taxiCost * 1.5;
	else
		taxiTotalCost = path.getSubwayAndTaxiDis() *  taxiCost;

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
		if(taxiTotalCost < subwayTotalCost)
			vehicle = taxi;
		else
			vehicle = subway;
	}
	else
		vehicle = bus;

	//check if new path is better or not
	if(vehicle == bus)
	{
		int result = busTotalCost + currentNode.costUntilNow;
		if(result < nextNodes[0].costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNodes.clear();
			nextNodes.push_back(Navigator::useBus(currentNode,path, startTime));
		}
		else if(result == nextNodes[0].costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNodes.push_back(Navigator::useBus(currentNode,path, startTime));
		}

	}
	else if(vehicle == subway)
	{
		int result = subwayTotalCost + currentNode.costUntilNow;
		if(result < nextNodes[0].costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNodes.clear();
			nextNodes.push_back(Navigator::useSubway(currentNode,path, startTime));
		}
	}
	else
	{
		if(taxiTotalCost + currentNode.costUntilNow < nextNodes[0].costUntilNow)
		{
			currentNode.paths.push_back(v);
			nextNodes.clear();
			nextNodes.push_back(Navigator::useTaxi(currentNode,path, startTime));
		}
	}
}

DijkstraNode  CostBaseNavigator::navigate(int src, int des, Clock startTime)
{
    cout << "starting cost base navigator..." << endl;

	vector<DijkstraNode> nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
	{
		DijkstraNode newNode;
		newNode.costUntilNow = INT_MAX;
		nodes[i].push_back(newNode);

		visited[i] = false;
	}

	// Distance of source vertex from itself is always 0
	nodes[src][0].costUntilNow = 0;
	nodes[src][0].paths.push_back(src);

	// Find shortest path for all vertices
	for (int count = 0; count < *stationsCount; count++) 
	{

		// Find the lowest between nodes that are not visited
		int u = minNode(nodes, visited);
		// Mark the picked vertex as processed
		visited[u] = true;
		cout << "check node "<< u << endl;
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

	return nodes[des][0];
}