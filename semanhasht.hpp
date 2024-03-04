#ifndef SMNHSHT_HPP
#define SMNHSHT_HPP
#include <iostream>
#include <queue>
#include "city.h"

struct Input
{
    std::string startTime;
    std::string origin;
    std::string destination; 
};


class Semanhasht
{
    public:
    Semanhasht();

    void getInput();
    void run();


    private:
    std::queue <Input>input;
    Input request;
    Clock start;
    int numberOfRequests;
};



#endif