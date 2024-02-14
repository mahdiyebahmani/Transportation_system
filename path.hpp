#ifdef PATH_H
#define PATH_H

class Path
{
    public:

    void set_bus(bool)
    void set_subway_and_taxi(bool)
    void set_subway_and_taxi_dis(double)
    void set_bus_dis(double)
    void set_subway_line(int)

    bool get_bus(bool)
    bool get_subway_and_taxi(bool)
    double get_subway_and_taxi_dis(double)
    double get_bus_dis(double)
    int get_subway_line(int);
    
    private:
    bool subway_and_taxi = 0;
    bool bus = 0;
    double subway_and_taxi_dis;
    double bus_dis;
    int subway_line;

};

#endif