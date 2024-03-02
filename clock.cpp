#include "clock.h"
using namespace std;

Clock::Clock() : hour{0}, minute{0} {}

Clock::Clock(int minutes)
{
    setFromMinute(minutes);
}

void Clock::setFromMinute(int minutes)
{
    int hour = 0;
    while(minutes >= 60)
    {
        hour++;
        minutes -= 60;

        if(hour >= 24)
            hour = 0;
    }

    minute = minutes;
}

void Clock::setFromString(string clock)
{
    if(clock.length() > 5)
        throw ("invalid clock");

    int tmp = -1;
    bool hourAssigned = 0;
    for(char ch : clock)
    {
        if(ch == ':')
        {
            setHour(tmp);
            tmp = -1;
        }
        else
        {
            int num = int(ch) - 48;
            if(num < 9 && num >= 0)
            {
                if(tmp != -1)
                    tmp += num;
                else
                    tmp += num * 10 + 1;
            }
        }
    }
    setMinute(tmp);
}

void Clock::setMinute(int minute)
{
    if(minute >= 60 || minute < 0)
        this->minute = 0;
    else
        this->minute = minute;
}

void Clock::setHour(int hour)
{
    if(hour >= 24 || hour < 0)
        this->hour = 0;
    else
        this->hour = hour;
}

void Clock::setInfinity()
{
    hour = 25;
}

void Clock::addHour(int hour)
{
    if(hour < 0)
        return;

    this->hour = (this->hour + hour) % 24;
}

void Clock::addMinute(int minute)
{
    if(minute < 0)
        return;

    this->minute = (this->minute + minute) % 60;
    addHour(int(minute / 60));
}

int Clock::getHour()
{
    return hour;
}

int Clock::getMinute()
{
    return minute;
}

int Clock::getInMinute()
{
    int result = (hour * 60) + minute;
    return result;
}

string Clock::getHourInString()
{
    string s;
    if(hour < 10)
        s += '0';
    s += to_string(hour);

    return s;
}

string Clock::getMinuteInString()
{
    string s;
    if(minute < 10)
        s += '0';
    s += to_string(minute);

    return s;
}

string Clock::getClockInString()
{
    string s;
    s += getHourInString();
    s += ':';
    s += getMinuteInString();

    return s;
}

bool Clock::getBusSubwayTrafficHour()
{
    return (hour > 6 && hour < 8);
}   

bool Clock::getTaxiTrafficHour()
{
    return (hour > 18 && hour < 20);
}

void Clock::sumation(Clock temp)
{
    addHour(temp.getHour());
    addMinute(temp.getMinute());
}