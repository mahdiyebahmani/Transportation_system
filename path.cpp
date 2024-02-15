#include "path.hpp"
#include <fstream>
#include <string>
#include <map>
#include <iostream>

using namespace std;

map<string,int>name_to_index;

void Path::set_bus(bool t)
{
    bus = t;
}

void Path::set_subway_and_taxi(bool t)
{
    subway_and_taxi = t;
}

void Path::set_subway_and_taxi_dis(double dis)
{
    subway_and_taxi_dis = dis;
}

void Path::set_bus_dis(double dis)
{
    bus_dis = dis;
}

void Path::set_subway_line(int line)
{
    subway_line = line;
}

bool Path::get_bus(bool t)
{
    return bus;
}

bool Path::get_subway_and_taxi(bool t)
{
    return subway_and_taxi;
}

double Path::get_subway_and_taxi_dis(double dis)
{
    return subway_and_taxi_dis;
}

double Path::get_bus_dis(double dis)
{
    return bus_dis;
}

int Path::get_subway_line(int line)
{
    return subway_line;
}

void Path::read_file()
{
    ifstream Taxi_in ("Taxi.txt");
    
    string node;
    int index;

    if(Taxi_in.is_open())
    {
        while(Taxi_in >> index >> node )
        {
            name_to_index
        .insert({node,index});
        }
        Taxi_in.close();
    }
    
    else 
    cout << "Error occured while opening file"<<endl;

    ifstream Bus_in ("Bus.txt");

    if(Bus_in.is_open())
    {
       while(Bus_in >> index >> node )
        {
            name_to_index
        .insert({node,index});
        }
        Bus_in.close();
    }

    else 
    cout << "Error occured while opening file"<<endl;
}

string Path::index_to_string_convertor(int key)
{
    for (auto i : name_to_index
)
    {
        if(i.second == key)
        {
            string value = i.first;
            return value;
        }
    }
    return " ";
}

int Path::string_to_index_convertor(string key)
{
    if(name_to_index
.find(key) != name_to_index
.end())
    {
        return name_to_index
    [key];
    }
    return 0;
}