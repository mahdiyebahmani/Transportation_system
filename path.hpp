#ifndef PATH_H
#define PATH_H

enum Vehicle {taxi, subway, bus};

class Path
{
    public:

    Path();
    Path(Vehicle, int, int);

    void setPath(Vehicle, int, int);

    int getSubwayAndTaxiDis();
    int getBusDis();
    int getSubwayLine();
    int getBusLine();

    private:
    int subwayAndTaxiDis = 0;
    int busDis = 0;
    int subwayLine;
    int busLine;

};

#endif