#pragma once
#include <string>
#include <vector>

class DateTime {
private:
    int m_Day, m_Month, m_Year;
    int m_Hour, m_Minute, m_Second;

    // Validation helper methods

    void ValidateAndSet(int day, int month, int year, int hour, int minute, int second);

public:
    static bool IsLeapYear(int year);

    static int GetDaysInMonth(int month, int year);

    static bool IsValidDate(int day, int month, int year);

    static bool IsValidTime(int hour, int minute, int second);

    static DateTime Now();

    DateTime();

    DateTime(int Day, int Month, int Year, int Hour = 0, int Minute = 0, int Second = 0);

    DateTime(const DateTime &datetime);

    ~DateTime() = default;

    DateTime &SetDay(int Day);

    DateTime &SetMonth(int Month);

    DateTime &SetYear(int Year);

    DateTime &SetHour(int Hour);

    DateTime &SetMinute(int Minute);

    DateTime &SetSecond(int Second);

    int GetDay() const;

    int GetMonth() const;

    int GetYear() const;

    int GetHour() const;

    int GetMinute() const;

    int GetSecond() const;

    bool operator <(const DateTime &other) const;

    bool operator >(const DateTime &other) const;

    bool operator >=(const DateTime &other) const;

    bool operator <=(const DateTime &other) const;

    std::string ToStringDate() const;

    std::string ToStringDateTime() const;

    time_t ToTimeT() const;

    static std::vector<int> GetValidMonths();

    static std::vector<int> GetValidDays(int month, int year);

    static std::vector<int> GetValidHours();

    static std::vector<int> GetValidMinutes(int interval = 5);

    static std::string MonthToString(int month);


    static DateTime FromDateTimeString(const std::string &str);

    static DateTime FromDateString(const std::string &str);

    // Add validation method
    bool IsValid() const;
};
