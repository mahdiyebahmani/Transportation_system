#include "navigator.h"
using namespace std;

void Navigator::init(int* stationsCount,std::map< std::pair<int,int>, Path >* paths, bool** adjencencyMatrix)
{
    this->stationsCount = stationsCount;
    this->paths = paths; 
    this->adjencencyMatrix = adjencencyMatrix;
}

int Navigator::minNode(DijkstraNode nodes[], bool visited[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < *stationsCount; v++)
		if (visited[v] == false && nodes[v].disToSource <= min)
			min = nodes[v].disToSource, min_index = v;

	return min_index;
}

DijkstraNode Navigator::useBus(DijkstraNode currentNode, Path path)
{
	if(currentNode.disToSource)
	{
		Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size() - 1];
		if(lastVehicle != bus)
		{
			currentNode.costUntilNow += busCost;
			currentNode.currentTime.addMinute(busDelay);
		}
	}
	else
	{
		currentNode.costUntilNow =  busCost;
		currentNode.currentTime.setMinute(busDelay);
	}
	if(currentNode.currentTime.getBusSubwayTrafficHour())
	currentNode.currentTime.addMinute(path.getBusDis() * busDuration * 2);
	else
	currentNode.currentTime.addMinute(path.getBusDis() *  busDuration);

	currentNode.vehicles.push_back(bus);
	currentNode.disToSource += path.getBusDis();
	currentNode.lastBusLine = path.getBusLine();
	return currentNode;
}

DijkstraNode Navigator::useTaxi(DijkstraNode currentNode, Path path)
{
	if(currentNode.disToSource)
	{
		Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size() - 1];
		if(lastVehicle != taxi)
		{
			currentNode.currentTime.addMinute(taxiDelay);
		}
	}
	else
	{
		currentNode.currentTime.setFromMinute(taxiDelay);
	}
	if(currentNode.currentTime.getTaxiTrafficHour())
	{
		currentNode.currentTime.addMinute(path.getSubwayAndTaxiDis() * busDuration * 2);
		currentNode.costUntilNow += (path.getSubwayAndTaxiDis() * taxiDuration * 1.5);
	}
	else
	{
		currentNode.currentTime.addMinute(path.getSubwayAndTaxiDis() *  taxiDuration);
		currentNode.costUntilNow += path.getSubwayAndTaxiDis() * taxiCost;
	}
	currentNode.vehicles.push_back(taxi);
	currentNode.disToSource += path.getSubwayAndTaxiDis();

	return currentNode;
}

DijkstraNode Navigator::useSubway(DijkstraNode currentNode, Path path)
{
	if(currentNode.disToSource)
	{
		Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size() - 1];
		if(lastVehicle != subway || currentNode.lastSubwayLine != path.getSubwayLine())
		{
			currentNode.costUntilNow +=  subwayCost;
			currentNode.currentTime.addMinute(subwayDelay);
		}
	}
	else
	{
		currentNode.costUntilNow =  subwayCost;
		currentNode.currentTime.setFromMinute(subwayDelay);
	}
	if(currentNode.currentTime.getBusSubwayTrafficHour())
	currentNode.currentTime.addMinute( path.getSubwayAndTaxiDis() *  subwayDuration * 2);
	else
	currentNode.currentTime.addMinute( path.getSubwayAndTaxiDis() *  subwayDuration);

	currentNode.vehicles.push_back(subway);
	currentNode.disToSource += path.getSubwayAndTaxiDis();
	currentNode.currentTime.addMinute( path.getSubwayAndTaxiDis() *  subwayDuration);
	currentNode.lastSubwayLine = path.getSubwayLine();
	return currentNode;
}

void Navigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = (*paths)[pathKey];


	//between bus path and subway/taxi path select min
	Vehicle vehicle;
	int busDis = path.getBusDis();
	int subwayDis = path.getSubwayAndTaxiDis();

	if(busDis && subwayDis)
	{
		if(busDis < subwayDis)
			vehicle = bus;
		else if(busDis > subwayDis)
			vehicle = subway;
		else{
			if(currentNode.vehicles.size() > 0 && currentNode.vehicles[currentNode.vehicles.size()-1] == bus)
				vehicle = bus;
			else
				vehicle = subway;
		}
	}
	else if(busDis)
		vehicle = bus;
	else
		vehicle = subway;

	//check if new path is better or not
	if(vehicle == bus)
	{
		//distance base
		if(busDis + currentNode.disToSource < nextNode.disToSource)
		{
			currentNode.paths.push_back(v);
			nextNode = useBus(currentNode,path);
		}

	}
	else
	{
		//distance base
		if(subwayDis + currentNode.disToSource < nextNode.disToSource)
		{
			currentNode.paths.push_back(v);
			nextNode = useSubway(currentNode,path);
		}
	}
}

DijkstraNode Navigator::navigate(int src, int des)
{
    cout << "starting dis base navigator..." << endl;

	DijkstraNode nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
		nodes[i].disToSource = INT_MAX, visited[i] = false;

	// Distance of source vertex from itself is always 0
	nodes[src].disToSource = 0;
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