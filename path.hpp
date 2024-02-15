#ifdef PATH_H
#define PATH_H

#include <string>
using namespace std;

class Path
{
    public:

    Path();
    Path(int);
    Path(int, int);

    void set_bus(bool);
    void set_subway_and_taxi(bool);
    void set_subway_and_taxi_dis(int);
    void set_bus_dis(int);
    void set_subway_line(int);

    bool get_bus(bool);
    bool get_subway_and_taxi(bool);
    int get_subway_and_taxi_dis(int);
    int get_bus_dis(int);
    int get_subway_line(int);

    private:
    bool subway_and_taxi = 0;
    bool bus = 0;
    int subway_and_taxi_dis;
    int bus_dis;
    int subway_line;

};

#endif