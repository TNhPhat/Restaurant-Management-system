#pragma once
#include <string>

class DateTime
{
private:
    int m_Day, m_Month, m_Year;
    int m_Hour, m_Minute, m_Second;
public:
    static DateTime Now();  
    DateTime();
    DateTime(int Day, int Month, int Year, int Hour = 0, int Minute = 0, int Second = 0);
    ~DateTime() = default;

    void SetDay(int Day);
    void SetMonth(int Month);
    void SetYear(int Year);
    void SetHour(int Hour);
    void SetMinute(int Minute);
    void SetSecond(int Second);

    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;
    int GetHour() const;
    int GetMinute() const;
    int GetSecond() const;

    std::string ToStringDate() const;
    std::string ToStringDateTime() const;

    static DateTime DateTime::FromDateTimeString(const std::string& str);
};
