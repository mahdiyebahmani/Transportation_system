#ifndef PATH_H
#define PATH_H

#include  <vector>

enum Vehicle {taxi, subway, bus};

struct DijkstraNode
{
    int disToSource{0};
    int currentTimeInMinute{0};
    int costUntilNow{0};
    std::vector<int> paths;
    std::vector<Vehicle> vehicles;
    int lastSubwayLine;
    int lastBusLine;
};

class Path
{
    public:

    Path();
    Path(Vehicle, int, int);

    void setPath(Vehicle, int, int);

    int getSubwayAndTaxiDis();
    int getBusDis();
    int getSubwayLine();
    int getBusLine();

    private:
    int subwayAndTaxiDis = 0;
    int busDis = 0;
    int subwayLine;
    int busLine;

};

#endif