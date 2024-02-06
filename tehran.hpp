// #ifdef TEHRAN_HPP
// #define TEHRAN_HPP
#include <iostream>
#include "path.hpp"
using namespace std;

class Tehran
{
    public:
    Tehran()
    {
        CreatCity();
    }
    void CreatCity();
    
    private:
    Path tehran[59][59]; 

};

// #endif