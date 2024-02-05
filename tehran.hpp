// #ifdef TEHRAN_HPP
// #define TEHRAN_HPP

#include<iostream>
using namespace std;

class Path
{
    private:
    int distance = 0;
    bool taxi,bus = 0;

    public:
    void Set_distance(int distance)
    {
        this->distance = distance;
    }
    void Set_Bus(bool T)
    {
        this->bus = 1;
    }
    void Set_taxi(bool T)
    {
        this->taxi = 1;
    }
};

class Tehran
{
    private:
    Path tehran[59][59]; 

    public:
    Tehran()
    {
        CreatCity();
    }
    void CreatCity();

};

// #endif