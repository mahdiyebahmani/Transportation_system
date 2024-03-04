#include "city.h"
using namespace std;

City::City()
{
    createCity();
}

bool City::readStations()
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

bool City::readTSLines()
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

bool City::readBusLines()
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

bool City::createCity()
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

    //create navigators
    disNavigator.init(&stationsCount, &paths, adjencencyMatrix);
    costBaseNavigator.init(&stationsCount, &paths, adjencencyMatrix);
    timeNavigator.init(&stationsCount, &paths, adjencencyMatrix);
	return 1;
}

string City::getPathDataAsString(DijkstraNode node, Clock startClock)
{
    stringstream log;
    
    startClock.addMinute(node.currentTimeInMinute);

    log << "\tdistance : " << node.disToSource << " km" << endl;
    log << "\tarive time : " << startClock.getClockInString() << endl;
    log << "\tcost : " << node.costUntilNow << " $" << endl;
    log << endl;

    while(!node.route.empty() && !node.vehicles.empty())
    {
        string vehicle;
        switch (node.vehicles.top())
        {
            case 0: vehicle = "taxi";
            break;
            case 1: vehicle = "subway";
            break;
            case 2: vehicle = "bus";
            default: 
            break;
        }
        node.vehicles.pop();

        log << '\t';
        log << stations[node.route.top()];
        node.route.pop();

        log << " --(" << vehicle << " line " << node.lines.top() << ")--> ";
        node.lines.pop();
        log << stations[node.route.top()] << endl;
    }

    return log.str();
}

string City::findBestPath(string src, string des, Clock time)
{
    stringstream log;

    int srcIndex, desIndex;
    for(int i=0; i < stationsCount; i++)
    {
        if(src == stations[i])
        {
            srcIndex = i;
        }
        if(des == stations[i])
        {
            desIndex = i;
        }
    }

    log << "---------------------------------------------------------" << endl;
    log << "Best Path from " << src << " to " << des << " (time:" << time.getClockInString() << ')'<< endl;

    //* dis base
    log << "\nShortest Way:\n\n";
    DijkstraNode d = disNavigator.navigate(desIndex, srcIndex, time);
    log << getPathDataAsString(d, time);


    // //* cost base
    log << "\nLowest Cost:\n\n";
    d = costBaseNavigator.navigate(desIndex, srcIndex, time);
    log << getPathDataAsString(d, time);

    // //* time base
    log << "\nBest Time:\n\n";
    d = timeNavigator.navigate(desIndex, srcIndex, time);
    log << getPathDataAsString(d, time);


    return log.str();
}