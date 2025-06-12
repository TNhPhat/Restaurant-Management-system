#pragma once

class DateTime
{
private:
    int m_Day, m_Month, m_Year;
    int m_Hour, m_Minute;
public:
    DateTime();
    DateTime(int Day, int Month, int Year);
    DateTime(int Day, int Month, int Year, int Hour, int Minute);
    ~DateTime() = default;
    void setDay(int Day);
    void setMonth(int Month);
    void setYear(int Year);
    void setHour(int Hour);
    void setMinute(int Minute);
    int getDay();
    int getMonth();
    int getYear();
    int getHour();
    int getMinute();
};
