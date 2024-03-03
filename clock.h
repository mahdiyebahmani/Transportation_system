#ifndef CLOCK
#define CLOCK

#include <string>

class Clock
{
    public:
    Clock();
    Clock(int);
    void setFromMinute(int);
    void setFromString(std::string);
    void setMinute(int);
    void setHour(int);
    void addHour(int);
    void addMinute(int);

    int getHour();
    int getMinute();
    int getInMinute();
    std::string getHourInString();
    std::string getMinuteInString();
    std::string getClockInString();

    bool isBusSubwayTrafficHour();
    bool isTaxiTrafficHour();

    private:
    int hour;
    int minute;
};

#endif