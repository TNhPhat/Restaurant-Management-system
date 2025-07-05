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
    DateTime(int Day, int Month, int Year, int Hour, int Minute, int Second);
    DateTime(const DateTime& datetime);
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

    bool operator < (const DateTime& other) const;
    bool operator > (const DateTime& other) const;
    bool operator >= (const DateTime& other) const;
    bool operator <= (const DateTime& other) const;

    std::string ToStringDate() const;
    std::string ToStringDateTime() const;
    time_t ToTimeT() const;

    static DateTime FromDateTimeString(const std::string& str);
    static DateTime FromDateString(const std::string& str);

};
