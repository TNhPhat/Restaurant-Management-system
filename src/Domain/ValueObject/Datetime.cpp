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

DateTime::DateTime() : m_Day(0), m_Month(0), m_Year(0), m_Hour(0), m_Minute(0), m_Second(0) {}

DateTime::DateTime(int Day, int Month, int Year, int Hour, int Minute, int Second)
{
    this->m_Day = Day;
    this->m_Month = Month;
    this->m_Year = Year;
    this->m_Hour = Hour;
    this->m_Minute = Minute;  
    this->m_Second = Second;
}
DateTime::DateTime(const DateTime& datetime){
    this->m_Day = datetime.GetDay();
    this->m_Month = datetime.GetMonth();
    this->m_Year = datetime.GetYear();
    this->m_Hour = datetime.GetHour();
    this->m_Minute = datetime.GetMinute();
    this->m_Second = datetime.GetSecond();
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

bool DateTime::operator < (const DateTime& other) const{
    if(this->m_Year == other.m_Year){
        if(this->m_Month == other.m_Month){
            if(this->m_Day == other.m_Day){
                if(this->m_Hour == other.m_Hour){
                    if(this->m_Minute == other.m_Minute)
                        return this->m_Second < other.m_Second;
                    return this->m_Minute < other.m_Minute;
                }
                return this->m_Hour < other.m_Hour;
            }
            return this->m_Day < other.m_Day;
        }
        return this->m_Month < other.m_Month;
    }
    return this->m_Year < other.m_Year;
}

bool DateTime::operator > (const DateTime& other) const{
    if(this->m_Year == other.m_Year){
        if(this->m_Month == other.m_Month){
            if(this->m_Day == other.m_Day){
                if(this->m_Hour == other.m_Hour){
                    if(this->m_Minute == other.m_Minute)
                        return this->m_Second > other.m_Second;
                    return this->m_Minute > other.m_Minute;
                }
                return this->m_Hour > other.m_Hour;
            }
            return this->m_Day > other.m_Day;
        }
        return this->m_Month > other.m_Month;
    }
    return this->m_Year > other.m_Year;
}

bool DateTime::operator >= (const DateTime& other) const{
    return !(*this < other);
}

bool DateTime::operator <= (const DateTime& other) const{
    return !(*this > other);
}

std::string DateTime::ToStringDate() const
{
    std::ostringstream oss;
    oss << m_Day << '/' << m_Month << '/' << m_Year;
    return oss.str();
}

std::string DateTime::ToStringDateTime() const
{
    std::ostringstream oss;
    oss << m_Day << '/' << m_Month << '/' << m_Year << ' '
        << std::setw(2) << std::setfill('0') << m_Hour << ':'
        << std::setw(2) << std::setfill('0') << m_Minute << ':'
        << std::setw(2) << std::setfill('0') << m_Second;
    return oss.str();
}

DateTime DateTime::FromDateTimeString(const std::string &str)
{
    int month, day, year, hour, minute, second = 0;  // default second to 0
    char sep1, sep2, sep3, sep4;

    std::istringstream iss(str);
    iss >> day >> sep1 >> month >> sep2 >> year >> hour >> sep3 >> minute;

    if (!iss || sep1 != '/' || sep2 != '/' || sep3 != ':') {
        throw std::invalid_argument("Invalid datetime format: " + str);
    }

    // Try to read seconds if they exist
    if (iss >> sep4 >> second) {
        if (sep4 != ':') {
            second = 0;  // Invalid separator, ignore seconds
        }
    }

    return DateTime(day, month, year, hour, minute, second);
}

DateTime DateTime::FromDateString(const std::string &str)
{
    int month, day, year, hour, minute, second;
    char sep1, sep2, space, sep3, sep4;

    std::istringstream iss(str);
    iss >> day >> sep1 >> month >> sep2 >> year;

    if (!iss || sep1 != '/' || sep2 != '/' ) {
        throw std::invalid_argument("Invalid date format: " + str);
    }

    return DateTime(day, month, year, hour, minute, second);
}

time_t DateTime::ToTimeT() const {
    std::tm timeinfo = {};
    timeinfo.tm_year = m_Year - 1900;  
    timeinfo.tm_mon  = m_Month - 1;   
    timeinfo.tm_mday = m_Day;
    timeinfo.tm_hour = m_Hour;
    timeinfo.tm_min  = m_Minute;
    timeinfo.tm_sec  = m_Second;

    return std::mktime(&timeinfo);     
}

