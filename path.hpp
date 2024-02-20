#ifndef PATH_H
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
    void set_subway_and_taxi_dis(double);
    void set_bus_dis(double);
    void set_subway_line(int);

    bool get_bus();
    bool get_subway_and_taxi();
    double get_subway_and_taxi_dis();
    double get_bus_dis();
    int get_subway_line();

    string index_to_string_convertor(int);
    int string_to_index_convertor(string);

    private:
    bool subway_and_taxi = 0;
    bool bus = 0;
    double subway_and_taxi_dis;
    double bus_dis;
    int subway_line;

};

#endif