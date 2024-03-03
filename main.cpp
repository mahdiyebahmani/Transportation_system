#include "city.h"
#include "clock.h"
#include <iostream>
using namespace std;

int main()
{
    City city;

    Clock start;
    start.setFromString("07:09");

    //cout << city.findBestPath("Aghdasiyeh", "ShahidMofatteh", start);
    cout << city.findBestPath("Aghdasiyeh", "EmamHossein", start);
}