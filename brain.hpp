#ifdef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include "path.hpp"
using namespace std;

class Brain
{
    public:
    
    void createCity();
    

    private:

    map< pair<int,int>, Path > stations;

};

#endif