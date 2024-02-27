#include "path.hpp"
using namespace std;

Path::Path(){}

Path::Path(Vehicle v, int dis, int line)
{
    setPath(v,dis,line);
}

void Path::setPath(Vehicle v, int dis, int line)
{
    if(v == bus)
    {
        busDis = dis;
        busLine = line;
    }
    else 
    {
        subwayAndTaxiDis = dis;
        subwayLine = line;
    }
}

int Path::getBusDis()
{
    return busDis;
}

int Path::getSubwayAndTaxiDis()
{
    return subwayAndTaxiDis;
}

int Path::getBusLine()
{
    return busLine;
}

int Path::getSubwayLine()
{
    return subwayLine;
}

