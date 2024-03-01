#include "city.h"
#include "clock.h"
#include <iostream>
using namespace std;

int main()
{
    City city;

    Clock start;
    start.setFromString("12:00");

    cout << city.findBestPath("EmamHossein", "Meydan-eAzadi", start);
}