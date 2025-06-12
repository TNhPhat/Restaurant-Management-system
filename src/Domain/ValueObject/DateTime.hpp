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
    void SetDay(int Day);
    void SetMonth(int Month);
    void SetYear(int Year);
    void SetHour(int Hour);
    void SetMinute(int Minute);
    int GetDay();
    int GetMonth();
    int GetYear();
    int GetHour();
    int GetMinute();
};
