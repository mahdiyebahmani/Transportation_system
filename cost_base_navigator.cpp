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

void CostBaseNavigator::setAllLine(int& line, int& nodeIndex, Vehicle v, map<int,bool>& visited, DijkstraNode nodes[], Clock& startTime)
{
	visited[nodeIndex] = 1;
	//int line = nodes[nodeIndex].lines.top();
	int cost = nodes[nodeIndex].costUntilNow;
	for(int i = 0; i < *stationsCount; i++)
	{
		if(adjencencyMatrix[nodeIndex][i] && !visited[i]) 		//neighbor
		{
			pair<int,int> pathKey(min(nodeIndex,i), max(nodeIndex,i));
			Path path = (*paths)[pathKey];

			if(v == bus && line == path.getBusLine())	//same line
			{
				if(cost < nodes[i].costUntilNow)
				{
					nodes[i] = Navigator::useBus(nodes[nodeIndex], path, startTime);
					nodes[i].route.push(i);
				}
				setAllLine(line, i, v, visited, nodes, startTime);
			}
			else if(v == subway && line == path.getSubwayLine())	//same line
			{
				if(cost < nodes[i].costUntilNow)
				{
					nodes[i] = Navigator::useSubway(nodes[nodeIndex], path, startTime);
					nodes[i].route.push(i);
				}
				setAllLine(line, i, v, visited, nodes, startTime);
			}
		}
	}
}

void CostBaseNavigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode, Clock startTime, DijkstraNode nodes[])
{
	if(nextNode.costUntilNow == INT_MAX)
	{
		//use vehicle to go
		int busTotalCost = busCost;
		int subwayTotalCost = subwayCost;

		pair<int,int> pathKey(min(u,v), max(u,v));
		Path path = (*paths)[pathKey];

		bool busAvailable = path.getBusDis();
		bool STAvailable = path.getSubwayAndTaxiDis();

		Vehicle vehicle;
		if(busAvailable && STAvailable)
		{
			if(busTotalCost < subwayTotalCost)
				vehicle = bus;
			else
				vehicle = subway;
		}
		else if(busAvailable)
			vehicle = bus;
		else 
			vehicle = subway;
		
		currentNode.route.push(v);

		int line;

		if(vehicle == bus)
			nodes[v] = Navigator::useBus(currentNode,path, startTime),
			line = path.getBusLine();
		else
			nodes[v] = Navigator::useSubway(currentNode,path, startTime),
			line = path.getSubwayLine();

		//set all line
		map<int, bool> visitedInLine;
		setAllLine(line, u, vehicle, visitedInLine, nodes, startTime);
	}
}

DijkstraNode  CostBaseNavigator::navigate(int src, int des, Clock startTime)
{
    cout << "starting cost base navigator..." << endl;

	DijkstraNode nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
	{
		nodes[i].costUntilNow = INT_MAX;

		visited[i] = false;
	}

	// Distance of source vertex from itself is always 0
	nodes[src].costUntilNow = 0;
	nodes[src].route.push(src);

	// Find shortest path for all vertices
	for (int count = 0; count < *stationsCount; count++) 
	{
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
				updateNode(u, v, nodes[u], nodes[v], startTime, nodes);
			}
		}
	}

	return nodes[des];
}