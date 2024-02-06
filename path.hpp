// #ifdef PATH_HPP
// #define PATH_HPP
#include <iostream>
using namespace std;
class Path
{
    public:
    void Set_distance(int distance)
    {
        this -> distance = distance;
    }
    void Set_Bus(bool T)
    {
        bus = 1;
    }
    void Set_taxi(bool T)
    {
        taxi = 1;
    }
    void Set_origin(string origin, int origin_index)
    {
        this -> origin = origin;
        this -> origin_index = origin_index;
    }
    void Set_destination(string destination, int destination_index )
    {
        this -> destination = destination;
        this -> destination_index = destination_index;
    }

    private:
    string origin;
    string destination;
    int origin_index;
    int destination_index;
    int distance = 0;
    bool taxi = 0;
    bool bus = 0;

};

// #endif