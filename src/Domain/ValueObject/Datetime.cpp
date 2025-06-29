#include"DateTime.hpp"
#include <sstream>
#include <chrono>
#include <iomanip>

DateTime DateTime::Now() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    return DateTime(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                    tm.tm_hour, tm.tm_min, tm.tm_sec);
}

DateTime::DateTime() : m_Day(1), m_Month(1), m_Year(1), m_Hour(0), m_Minute(0) {}

DateTime::DateTime(int Day, int Month, int Year, int Hour = 0, int Minute = 0, int Second = 0)
{
    this->m_Day = Day;
    this->m_Month = Month;
    this->m_Year = Year;
    this->m_Hour = Hour;
    this->m_Minute = Minute;  
    this->m_Second = Second;
}

void DateTime::SetDay(int Day)
{
    this->m_Day = Day;
}

void DateTime::SetMonth(int Month)
{
    this->m_Month = Month;
}

void DateTime::SetYear(int Year)
{
    this->m_Year = Year;
}

void DateTime::SetHour(int Hour)
{
    this->m_Hour = Hour;
}

void DateTime::SetMinute(int Minute)
{
    this->m_Minute = Minute;
}

void DateTime::SetSecond(int Second)
{
    this->m_Second = Second;
}

int DateTime::GetDay() const
{
    return this->m_Day;
}

int DateTime::GetMonth() const
{
    return this->m_Month;
}

int DateTime::GetYear() const
{
    return this->m_Year;
}

int DateTime::GetHour() const
{
    return this->m_Hour;
}

int DateTime::GetMinute() const
{
    return this->m_Minute;
}

int DateTime::GetSecond() const 
{
    return this->m_Second;
}

std::string DateTime::ToStringDate() const
{
    std::ostringstream oss;
    oss << m_Month << '/' << m_Day << '/' << m_Year;
    return oss.str();
}

std::string DateTime::ToStringDateTime() const
{
    std::ostringstream oss;
    oss << m_Month << '/' << m_Day << '/' << m_Year << ' '
        << std::setw(2) << std::setfill('0') << m_Hour << ':'
        << std::setw(2) << std::setfill('0') << m_Minute << ':'
        << std::setw(2) << std::setfill('0') << m_Second;
    return oss.str();
}

DateTime DateTime::FromDateTimeString(const std::string &str)
{
    int month, day, year, hour, minute, second;
    char sep1, sep2, space, sep3, sep4;

    std::istringstream iss(str);
    iss >> month >> sep1 >> day >> sep2 >> year >> space >> hour >> sep3 >> minute >> sep4 >> second;

    if (!iss || sep1 != '/' || sep2 != '/' || space != ' ' || sep3 != ':' || sep4 != ':') {
        throw std::invalid_argument("Invalid datetime format: " + str);
    }

    return DateTime(day, month, year, hour, minute, second);
}
