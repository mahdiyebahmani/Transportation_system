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


void TimeBaseNavigator::updateNode(int& u, int& v, DijkstraNode currentNode, DijkstraNode& nextNode, Clock startTime, int disFromLastStation[], bool startInSubTraffic[])
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
	disFromLastStation[v] = 0;
	if(currentNode.vehicles.empty() == false)
	{
		bool displacement = 1;
		Vehicle lastVehicle = currentNode.vehicles.top();
		if(lastVehicle == bus && path.getBusLine() == currentNode.lines.top())
			busTotalDuration -= busDelay,
			displacement = 0;
		else if(lastVehicle == subway && path.getSubwayLine() == currentNode.lines.top())
			subwayTotalDuration -= subwayNewDelay,
			displacement = 0;
        else if(lastVehicle == taxi && path.getSubwayLine() == currentNode.lines.top())
            taxiTotalDuration -= taxiDelay,
			displacement = 0;

		if(displacement)
		{
			if(startTime.isBusSubwayTrafficHour())
				startInSubTraffic[v] = 1;
		}
		else
		{
			disFromLastStation[v] = disFromLastStation[u];
			startInSubTraffic[v] = startInSubTraffic[u];
		}
	}
	else
	{
		if(startTime.isBusSubwayTrafficHour())
			startInSubTraffic[u] = 1,
			startInSubTraffic[v] = 1;
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

	//check dis from last station
	if(vehicle == bus)
		disFromLastStation[v] += path.getBusDis();
	else
		disFromLastStation[v] += path.getSubwayAndTaxiDis();

	if(u == 18 && v == 43)
	{
		cout << "taxiTotalDuration: " << taxiTotalDuration << " sub:" << subwayTotalDuration << " subTraffic: " << startInSubTraffic[u] << "disFromLastStartion: " << disFromLastStation[v] << endl;
	}
	if(u == 43 && v == 2)
	{
		cout << "taxiTotalDuration: " << taxiTotalDuration << " sub:" << subwayTotalDuration << " subTraffic: " << startInSubTraffic[u] << "disFromLastStartion: " << disFromLastStation[v] << endl;
	}
	if(u == 2 && v == 1)
	{
		cout << "taxiTotalDuration: " << taxiTotalDuration << " sub:" << subwayTotalDuration << " subTraffic: " << startInSubTraffic[u] << "disFromLastStartion: " << disFromLastStation[v] << endl;
	}
	if(startInSubTraffic[u])
	{
		if(vehicle == taxi && disFromLastStation[v] > 19)
		{
			//change vehcile from first of line until now to subway
			int currentLine = currentNode.lines.top();
			int count{0};
			while(!currentNode.lines.empty() && currentNode.lines.top() == currentLine
				&& currentNode.vehicles.top() == taxi)
			{
				currentNode.vehicles.pop();
				currentNode.lines.pop();
				count++;
			}

			while(count--)
			{
				currentNode.vehicles.push(subway);
				currentNode.lines.push(currentLine);
			}

			//change cost from first of line until now
			int c = (disFromLastStation[v] - path.getSubwayAndTaxiDis()) * taxiCost;
			currentNode.costUntilNow -= c;		
			currentNode.costUntilNow += subwayCost;

			//change duration from first of line until now
			int d = (disFromLastStation[v] - path.getSubwayAndTaxiDis()) * taxiDuration + taxiDelay;
			currentNode.currentTimeInMinute -= d;
			d = (disFromLastStation[v] - path.getSubwayAndTaxiDis()) * subwayDuration + subwayDelay;
			currentNode.currentTimeInMinute += d;

			//set next vehicle to subway too
			vehicle = subway;
		}
	}
	else
	{

		if(vehicle == taxi && disFromLastStation[v] > 3)
		{
			//change vehcile from first of line until now to subway
			int currentLine = currentNode.lines.top();
			int count{0};
			while(!currentNode.lines.empty() && currentNode.lines.top() == currentLine
				&& currentNode.vehicles.top() == taxi)
			{
				currentNode.vehicles.pop();
				currentNode.lines.pop();
				count++;
			}

			while(count--)
			{
				currentNode.vehicles.push(subway);
				currentNode.lines.push(currentLine);
			}

			//change cost from first of line until now
			int c = (disFromLastStation[v] - path.getSubwayAndTaxiDis()) * taxiCost;
			currentNode.costUntilNow -= c;		
			currentNode.costUntilNow += subwayCost;

			//change duration from first of line until now
			int d = (disFromLastStation[v] - path.getSubwayAndTaxiDis()) * taxiDuration + taxiDelay;
			currentNode.currentTimeInMinute -= d;
			d = (disFromLastStation[v] - path.getSubwayAndTaxiDis()) * subwayDuration + subwayDelay;
			currentNode.currentTimeInMinute += d;

			//set next vehicle to subway too
			vehicle = subway;
		}
	}

	//check if new path is better or not
	if(vehicle == bus)
	{
		if(busTotalDuration + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
		{
			currentNode.route.push(v);
			nextNode = Navigator::useBus(currentNode,path, startTime);
		}

	}
	else if(vehicle == subway)
	{
		if(subwayTotalDuration + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
		{
			currentNode.route.push(v);
			nextNode = Navigator::useSubway(currentNode,path, startTime);
		}
	}
    else
    {
        if(taxiTotalDuration + currentNode.currentTimeInMinute < nextNode.currentTimeInMinute)
		{
			currentNode.route.push(v);
			nextNode = Navigator::useTaxi(currentNode,path, startTime);
		}
    }
}

DijkstraNode  TimeBaseNavigator::navigate(int src, int des, Clock startTime)
{
	DijkstraNode nodes[*stationsCount];
	int disFromLastStation[*stationsCount]{0};
	bool startInSubTraffic[*stationsCount]{0};

	bool visited[*stationsCount];

	for (int i = 0; i < *stationsCount; i++)
		nodes[i].currentTimeInMinute = INT_MAX, visited[i] = false;


	nodes[src].currentTimeInMinute = 0;
	nodes[src].route.push(src);


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
				updateNode(u, v, nodes[u], nodes[v], startTime, disFromLastStation, startInSubTraffic);
			}
		}
	}

	return nodes[des];
}