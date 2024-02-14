// #ifdef TEHRAN_HPP
// #define TEHRAN_HPP
#include <iostream>
#include "path.hpp"
using namespace std;

class Brain
{
    public:
    Brain()
    {
        CreatCity();
    }
    void CreatCity();
    
    private:
    Path tehran[59][59]; 

};

// #endif