#include "semanhasht.hpp"

using namespace std;

Semanhasht::Semanhasht()
{

}

void Semanhasht::getInput()
{
    cout << "Enter number of requests"<<endl<<endl;
    cin >> numberOfRequests;
    for(int i = 0; i < numberOfRequests; i++)
    {   
        cout<< "enter origin/destination/start time"<<endl;
        cin >> request.origin >> request.destination >>request.startTime;
        input.push(request);
    }
}

void Semanhasht::run()
{
    getInput();
    City city;
    for(int i = 0; i< numberOfRequests; i++)
    {
        request = input.front();   
        start.setFromString(request.startTime);
        cout << city.findBestPath(request.origin, request.destination, start) <<endl<<endl;
        input.pop();
    }
    
}
