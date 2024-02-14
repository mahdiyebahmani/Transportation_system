#include <iostream>
#include <map>
#include <fstream>
#include "tehran.hpp"
using namespace std;

void Brain::CreatCity()
{
    std::ifstream Taxi_in ("Taxi.txt");
    std::ifstream Bus_in ("Bus.txt");
    map <string,int> find_index;
    Path temp;
    string origin;
    string destination;
    int origin_index;
    int destination_index;
    int distance = 0;

    if(Taxi_in.is_open())
    {
       
        while(Taxi_in >> origin_index >> origin >> destination_index >> destination >> distance)
        {
            temp.Set_taxi(1);
            temp.Set_origin(origin, origin_index);
            temp.Set_destination(destination, destination_index);
            temp.Set_distance(distance);
            tehran[origin_index][destination_index] = temp;
            tehran[destination_index][origin_index] = temp;

            if (find_index.find(origin) == find_index.end())
            {
                find_index.insert({origin, origin_index});
            }

            if (find_index.find(destination) == find_index.end())
            {
                find_index.insert({destination, destination_index});
            }
        }
    }

    else 
    cout << "Error occured while opening file"<<endl;


    if(Bus_in.is_open())
    {
        while(Bus_in >> origin_index >> origin >> destination_index >> destination >> distance)
        {
            temp.Set_Bus(1);
            temp.Set_origin(origin, origin_index);
            temp.Set_destination(destination, destination_index);
            temp.Set_distance(distance);
            tehran[origin_index][destination_index] = temp;
            tehran[destination_index][origin_index] = temp;

            if (find_index.find(origin) == find_index.end())
            {
                find_index.insert({origin, origin_index});
            }

            if (find_index.find(destination) == find_index.end())
            {
                find_index.insert({destination, destination_index});
            }
        }
    }

    else 
    cout << "Error occured while opening file"<<endl;

}

int main()
{
    Brain test;
}


