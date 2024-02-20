#include "path.hpp"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

Path::Path(){}

Path::Path(int busDis)
{
    set_bus(true);
    set_bus_dis(busDis);
}

Path::Path(int subwayAndTaxiDis, int subwayLine)
{
    set_subway_and_taxi(true);
    set_subway_and_taxi_dis(subwayAndTaxiDis);
    set_subway_line(subway_line);
}

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

bool Path::get_bus()
{
    return bus;
}

bool Path::get_subway_and_taxi()
{
    return subway_and_taxi;
}

double Path::get_subway_and_taxi_dis()
{
    return subway_and_taxi_dis;
}

double Path::get_bus_dis()
{
    return bus_dis;
}

int Path::get_subway_line()
{
    return subway_line;
}

