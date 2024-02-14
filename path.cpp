#include "path.hpp"

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