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

int Brain::minDistance(DijkstraNode nodes[], bool visited[])
{
	// Initialize min value
	int min = INT_MAX, min_index;

	for (int v = 0; v < stationsCount; v++)
		if (visited[v] == false && nodes[v].disToSource <= min)
			min = nodes[v].disToSource, min_index = v;

	return min_index;
}

int Brain::minCost(DijkstraNode nodes[], bool visited[])
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < stationsCount; v++)
		if (visited[v] == false && nodes[v].costUntilNow <= min)
			min = nodes[v].costUntilNow, min_index = v;

	return min_index;
}

int Brain::minTime(DijkstraNode nodes[], bool visited[])
{
	int min = INT_MAX, min_index;

	for (int v = 0; v < stationsCount; v++)
		if (visited[v] == false && nodes[v].currentTimeInMinute <= min)
			min = nodes[v].currentTimeInMinute, min_index = v;

	return min_index;
}

DijkstraNode Brain::useBus(DijkstraNode lastNode, Path path)
{
	if(lastNode.disToSource)
	{
		Vehicle lastVehicle = lastNode.vehicles[lastNode.vehicles.size() - 1];
		if(lastVehicle != bus)
		{
			lastNode.costUntilNow += busCost;
			lastNode.currentTimeInMinute += busDelay;
		}
	}
	else
	{
		lastNode.costUntilNow = busCost;
		lastNode.currentTimeInMinute = busDelay;
	}
	lastNode.vehicles.push_back(bus);
	lastNode.disToSource += path.getBusDis();
	lastNode.currentTimeInMinute += path.getBusDis() * busDuration;

	return lastNode;
}

DijkstraNode Brain::useTaxi(DijkstraNode lastNode, Path path)
{
	if(lastNode.disToSource)
	{
		Vehicle lastVehicle = lastNode.vehicles[lastNode.vehicles.size() - 1];
		if(lastVehicle != taxi)
		{
			lastNode.currentTimeInMinute += taxiDelay;
		}
	}
	else
	{
		lastNode.currentTimeInMinute = taxiDelay;
	}
	lastNode.vehicles.push_back(taxi);
	lastNode.disToSource += path.getSubwayAndTaxiDis();
	lastNode.currentTimeInMinute += path.getSubwayAndTaxiDis() * taxiDuration;
	lastNode.costUntilNow += path.getSubwayAndTaxiDis() * subwayCost;

	return lastNode;
}

DijkstraNode Brain::useSubway(DijkstraNode lastNode, Path path)
{
	if(lastNode.disToSource)
	{
		Vehicle lastVehicle = lastNode.vehicles[lastNode.vehicles.size() - 1];
		if(lastVehicle != subway || lastNode.lastSubwayLine != path.getSubwayLine())
		{
			lastNode.costUntilNow += subwayCost;
			lastNode.currentTimeInMinute += subwayDelay;
		}
	}
	else
	{
		lastNode.costUntilNow = subwayCost;
		lastNode.currentTimeInMinute = subwayDelay;
	}
	lastNode.vehicles.push_back(subway);
	lastNode.disToSource += path.getSubwayAndTaxiDis();
	lastNode.currentTimeInMinute += path.getSubwayAndTaxiDis() * subwayDuration;

	return lastNode;
}

void Brain::updateNodeDistance(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = paths[pathKey];


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


void Brain::updateNodeTime(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = paths[pathKey];

	//between bus path and subway/taxi path select min
	Vehicle vehicle;
	int busTime = path.getBusDis() * busDuration;
	int subwayTime = path.getSubwayAndTaxiDis() * subwayDuration;
	int taxiTime = path.getSubwayAndTaxiDis() * taxiDuration;
	
	bool flag = 0 ;

	Vehicle lastVehicle = currentNode.vehicles[currentNode.vehicles.size()-1];

	if(busTime + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		if(lastVehicle!= bus) 
		{
			busTime += busDelay;
		}
	}
	if(subwayTime + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		if(lastVehicle != subway)
		{
			subwayTime += subwayDelay;
		}
	}
	if(taxiTime + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		if(lastVehicle != taxi)
		{
			taxiTime += taxiDelay;
		}
	}

	if(busTime + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		currentNode.paths.push_back(v);
		nextNode.vehicles = currentNode.vehicles;
		nextNode.vehicles.push_back(bus);
		nextNode.disToSource = currentNode.disToSource + path.getBusDis();
		nextNode.currentTimeInMinute = currentNode.currentTimeInMinute + busTime;
		if(currentNode.lastBusLine == path.getBusLine())
		nextNode.costUntilNow = busCost;
		else
		nextNode.costUntilNow += busCost;
	}

	if(subwayTime + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		currentNode.paths.push_back(v);
		nextNode.vehicles = currentNode.vehicles;
		nextNode.vehicles.push_back(subway);
		nextNode.disToSource = nextNode.disToSource + path.getSubwayAndTaxiDis();
		nextNode.currentTimeInMinute = nextNode.currentTimeInMinute + subwayTime;
		if(currentNode.lastSubwayLine == path.getSubwayLine() )
		nextNode.costUntilNow = subwayCost;
		else 
		nextNode.costUntilNow = currentNode.costUntilNow + subwayCost;
	}
	
	if(taxiTime + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		currentNode.paths.push_back(v);
		nextNode.vehicles = currentNode.vehicles;
		nextNode.vehicles.push_back(taxi);
		nextNode.disToSource = currentNode.disToSource + path.getSubwayAndTaxiDis();
		nextNode.currentTimeInMinute = currentNode.currentTimeInMinute + taxiTime;
		nextNode.costUntilNow = currentNode.costUntilNow + (path.getSubwayAndTaxiDis() * taxiCost);
	}
}

void Brain::updateNodeCost(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode)
{
	pair<int,int> pathKey(min(u,v), max(u,v));
	Path path = paths[pathKey];
	
	Vehicle lastvehicle = currentNode.vehicles[currentNode.vehicles.size() - 1];

	int taxiPayment = path.getSubwayAndTaxiDis() * taxiCost;
	int busPayment = 0, subwayPayment = 0;
	
	if(currentNode.vehicles.empty())
	{
		busPayment = busCost;
		subwayPayment = subwayCost;
	}
	else if(lastvehicle != bus)
	{
		busPayment += busCost;
	}
	else if(lastvehicle != subway)
	{
		subwayPayment += subwayDelay;
	}
	else if(lastvehicle != taxi)
	{
		taxiPayment += taxiDelay;
	}
	
	if(busPayment + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		currentNode.paths.push_back(v);
		nextNode.vehicles = currentNode.vehicles;
		nextNode.vehicles.push_back(bus);
		nextNode.disToSource = currentNode.disToSource + path.getBusDis();
		nextNode.currentTimeInMinute = currentNode.currentTimeInMinute + (currentNode.disToSource * busDuration);
		if(lastvehicle != bus || path.getBusLine() != currentNode.lastBusLine)
		nextNode.currentTimeInMinute += busDelay;
		nextNode.costUntilNow = currentNode.costUntilNow + busCost;
	}

	if(subwayPayment + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		currentNode.paths.push_back(v);
		nextNode.vehicles = currentNode.vehicles;
		nextNode.vehicles.push_back(subway);
		nextNode.disToSource = nextNode.disToSource + path.getSubwayAndTaxiDis();
		nextNode.currentTimeInMinute = nextNode.currentTimeInMinute + (currentNode.disToSource * subwayDuration);
		if(lastvehicle != subway || path.getSubwayLine() != currentNode.lastSubwayLine)
		nextNode.currentTimeInMinute += subwayDelay;
		nextNode.costUntilNow = subwayPayment;
	}
	
	if(taxiPayment + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
	{
		currentNode.paths.push_back(v);
		nextNode.vehicles = currentNode.vehicles;
		nextNode.vehicles.push_back(taxi);
		nextNode.disToSource = currentNode.disToSource + path.getSubwayAndTaxiDis();
		nextNode.currentTimeInMinute = currentNode.currentTimeInMinute + (currentNode.disToSource * taxiDuration);
		if(lastvehicle != taxi)
		nextNode.currentTimeInMinute += taxiDelay;
		nextNode.costUntilNow = currentNode.costUntilNow + (path.getSubwayAndTaxiDis() * taxiCost);
	}
}

DijkstraNode Brain::dijkstra(int src, int des, int base)
{
	cout << "starting dijkstra algorithm" << endl;

	DijkstraNode nodes[stationsCount];

	bool visited[stationsCount];

	for (int i = 0; i < stationsCount; i++)
	{
		if(base == 1)
			nodes[i].base = nodes[i].disToSource;

		else if(base == 2)
			nodes[i].base = nodes[i].currentTimeInMinute;

		else 
			nodes[i].base = nodes[i].costUntilNow;

		nodes[i].base = INT_MAX, visited[i] = false;
	}
		

	// Distance of source vertex from itself is always 0
	nodes[src].base = 0;

	// Find shortest path for all vertices
	cout << "finding shortest path for all stations..." << endl;
	for (int count = 0; count < stationsCount; count++) {
		int u;
		// Find the lowest between nodes that are not visited
		if(base == 1)
			u = minDistance(nodes, visited);

		else if(base == 2)
			u = minCost(nodes, visited);

		else if(base == 3)
			u = minTime(nodes, visited);

		// Mark the picked vertex as processed
		visited[u] = true;

		// Update node value of the adjacent vertices of the picked vertex.
		for (int v = 0; v < stationsCount; v++)
		{
			// Update node[v]
			if (adjencencyMatrix[u][v] && !visited[v])
			{
				if(base == 1)
					updateNodeDistance(u, v, nodes[u], nodes[v]);

				if(base == 2)
					updateNodeCost(u, v, nodes[u], nodes[v]);

				if(base == 3)
					updateNodeTime(u, v, nodes[u], nodes[v]);
			}
		}
	}

	cout << "return answer" << endl;
	return nodes[des];
}
