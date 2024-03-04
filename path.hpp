#ifndef PATH_H
#define PATH_H

#include  <vector>
#include <stack>

enum Vehicle {taxi, subway, bus};

struct DijkstraNode
{
    int disToSource{0};
    int currentTimeInMinute{0};
    int costUntilNow{0};
    std::stack<int> route;
    std::stack<Vehicle> vehicles;
    std::stack<int> lines;
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