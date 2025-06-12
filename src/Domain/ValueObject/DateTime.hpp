#pragma once

class DateTime
{
private:
    int m_Day, m_Month, m_Year;
    int m_Hour, m_Minute;
public:
    DateTime(): m_Day(1), m_Month(1), m_Year(1), m_Hour(0), m_Minute(0);
    DateTime(int day, int month, int year);
    DateTime(int day, int month, int year, int hour, int minute);
    void setDay(int day);
    void setMonth(int month);
    void setYear(int year);
    void setHour(int hour);
    void setMinute(int minute);
    int getDay();
    int getMonth();
    int getYear();
    int getHour();
    int getMinute();
};
