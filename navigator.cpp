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

DijkstraNode Navigator::useBus(DijkstraNode currentNode, Path path, Clock startTime)
{
	//check traffic time
	int newDelay = busDelay;
	startTime.addMinute(currentNode.currentTimeInMinute);
	if(startTime.isBusSubwayTrafficHour())
		currentNode.currentTimeInMinute += path.getBusDis() *  busDuration * 2,
		newDelay *= 2;
	else
		currentNode.currentTimeInMinute += path.getBusDis() *  busDuration;

	if(currentNode.vehicles.empty() == false)
	{
		Vehicle lastVehicle = currentNode.vehicles.top();
		if(lastVehicle != bus)
		{
			currentNode.costUntilNow += busCost;
			currentNode.currentTimeInMinute +=  newDelay;
		}
	}
	else
	{
		currentNode.costUntilNow +=  busCost;
		currentNode.currentTimeInMinute +=  newDelay;
	}

	currentNode.vehicles.push(bus);
	currentNode.disToSource += path.getBusDis();
	currentNode.lines.push(path.getBusLine());
	return currentNode;
}

DijkstraNode Navigator::useTaxi(DijkstraNode currentNode, Path path, Clock startTime)
{
	//check traffic time
	int newDelay = taxiDelay;
	startTime.addMinute(currentNode.currentTimeInMinute);
	if(startTime.isTaxiTrafficHour())
		currentNode.currentTimeInMinute += path.getSubwayAndTaxiDis() *  taxiDuration * 2,
		currentNode.costUntilNow += path.getSubwayAndTaxiDis() *  taxiCost * 1.5,
		newDelay *= 2;
	else
		currentNode.currentTimeInMinute += path.getSubwayAndTaxiDis() *  taxiDuration,
		currentNode.costUntilNow += path.getSubwayAndTaxiDis() *  taxiCost;
	
	if(currentNode.vehicles.empty() == false)
	{
		Vehicle lastVehicle = currentNode.vehicles.top();
		if(lastVehicle != taxi)
		{
			currentNode.currentTimeInMinute +=  newDelay;
		}
	}
	else
	{
		currentNode.currentTimeInMinute +=  newDelay;
	}

	currentNode.vehicles.push(taxi);
	currentNode.disToSource += path.getSubwayAndTaxiDis();
	currentNode.lines.push(path.getSubwayLine());

	return currentNode;
}

DijkstraNode Navigator::useSubway(DijkstraNode currentNode, Path path, Clock startTime)
{
	if(currentNode.vehicles.empty() == false)
	{
		Vehicle lastVehicle = currentNode.vehicles.top();
		if(lastVehicle != subway || currentNode.lines.top() != path.getSubwayLine())
		{
			currentNode.costUntilNow +=  subwayCost;

			//check traffic time
			startTime.addMinute(currentNode.currentTimeInMinute);
			if(startTime.isBusSubwayTrafficHour())
				currentNode.currentTimeInMinute +=  subwayDelay * 3;
			else
				currentNode.currentTimeInMinute +=  subwayDelay;

		}
	}
	else
	{
		currentNode.costUntilNow +=  subwayCost;

		//check traffic time
		startTime.addMinute(currentNode.currentTimeInMinute);
		if(startTime.isBusSubwayTrafficHour())
			currentNode.currentTimeInMinute +=  subwayDelay * 3;
		else
			currentNode.currentTimeInMinute +=  subwayDelay;
	}
	
	currentNode.vehicles.push(subway);
	currentNode.disToSource += path.getSubwayAndTaxiDis();
	currentNode.lines.push(path.getSubwayLine());
	currentNode.currentTimeInMinute += path.getSubwayAndTaxiDis() *  subwayDuration;
	return currentNode;
}

void Navigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode, Clock startTime)
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
			if(currentNode.vehicles.empty() == false && currentNode.vehicles.top() == bus)
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
			currentNode.route.push(v);
			nextNode = useBus(currentNode,path, startTime);
		}

	}
	else
	{
		//distance base
		if(subwayDis + currentNode.disToSource < nextNode.disToSource)
		{
			currentNode.route.push(v);
			nextNode = useSubway(currentNode,path, startTime);
		}
	}
}

DijkstraNode Navigator::navigate(int src, int des, Clock startTime)
{
    cout << "starting dis base navigator..." << endl;

	DijkstraNode nodes[*stationsCount];

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
		nodes[i].disToSource = INT_MAX, visited[i] = false;

	// Distance of source vertex from itself is always 0
	nodes[src].disToSource = 0;
	nodes[src].route.push(src);

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
				updateNode(u, v, nodes[u], nodes[v], startTime);
			}
		}
	}

	return nodes[des];
}