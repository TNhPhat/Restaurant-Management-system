#include"DateTime.hpp"
#include <sstream>
#include <chrono>
#include <iomanip>

// Validation helper methods
bool DateTime::IsLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int DateTime::GetDaysInMonth(int month, int year) {
    if (month < 1 || month > 12) return 0;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && IsLeapYear(year)) {
        return 29;
    }
    return daysInMonth[month - 1];
}

bool DateTime::IsValidDate(int day, int month, int year) {
    if (year < 1) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > GetDaysInMonth(month, year)) return false;
    return true;
}

bool DateTime::IsValidTime(int hour, int minute, int second) {
    if (hour < 0 || hour > 23) return false;
    if (minute < 0 || minute > 59) return false;
    if (second < 0 || second > 59) return false;
    return true;
}

void DateTime::ValidateAndSet(int day, int month, int year, int hour, int minute, int second) {
    if (!IsValidDate(day, month, year)) {
        throw std::invalid_argument("Invalid date: " + std::to_string(day) + "/" +
                                    std::to_string(month) + "/" + std::to_string(year));
    }
    if (!IsValidTime(hour, minute, second)) {
        throw std::invalid_argument("Invalid time: " + std::to_string(hour) + ":" +
                                    std::to_string(minute) + ":" + std::to_string(second));
    }

    m_Day = day;
    m_Month = month;
    m_Year = year;
    m_Hour = hour;
    m_Minute = minute;
    m_Second = second;
}

bool DateTime::IsValid() const {
    return IsValidDate(m_Day, m_Month, m_Year) && IsValidTime(m_Hour, m_Minute, m_Second);
}

DateTime DateTime::Now() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);
    return DateTime(tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
                    tm.tm_hour, tm.tm_min, tm.tm_sec);
}

DateTime::DateTime() : m_Day(1), m_Month(1), m_Year(1900), m_Hour(0), m_Minute(0), m_Second(0) {
}

DateTime::DateTime(int Day, int Month, int Year, int Hour, int Minute, int Second) {
    ValidateAndSet(Day, Month, Year, Hour, Minute, Second);
}

DateTime::DateTime(const DateTime &datetime) {
    this->m_Day = datetime.GetDay();
    this->m_Month = datetime.GetMonth();
    this->m_Year = datetime.GetYear();
    this->m_Hour = datetime.GetHour();
    this->m_Minute = datetime.GetMinute();
    this->m_Second = datetime.GetSecond();
}

void DateTime::SetDay(int Day) {
    if (!IsValidDate(Day, m_Month, m_Year)) {
        throw std::invalid_argument("Invalid day: " + std::to_string(Day) +
                                    " for month " + std::to_string(m_Month) +
                                    " and year " + std::to_string(m_Year));
    }
    this->m_Day = Day;
}

void DateTime::SetMonth(int Month) {
    if (!IsValidDate(m_Day, Month, m_Year)) {
        throw std::invalid_argument("Invalid month: " + std::to_string(Month) +
                                    " for day " + std::to_string(m_Day) +
                                    " and year " + std::to_string(m_Year));
    }
    this->m_Month = Month;
}

void DateTime::SetYear(int Year) {
    if (!IsValidDate(m_Day, m_Month, Year)) {
        throw std::invalid_argument("Invalid year: " + std::to_string(Year) +
                                    " for day " + std::to_string(m_Day) +
                                    " and month " + std::to_string(m_Month));
    }
    this->m_Year = Year;
}

void DateTime::SetHour(int Hour) {
    if (!IsValidTime(Hour, m_Minute, m_Second)) {
        throw std::invalid_argument("Invalid hour: " + std::to_string(Hour));
    }
    this->m_Hour = Hour;
}

void DateTime::SetMinute(int Minute) {
    if (!IsValidTime(m_Hour, Minute, m_Second)) {
        throw std::invalid_argument("Invalid minute: " + std::to_string(Minute));
    }
    this->m_Minute = Minute;
}

void DateTime::SetSecond(int Second) {
    if (!IsValidTime(m_Hour, m_Minute, Second)) {
        throw std::invalid_argument("Invalid second: " + std::to_string(Second));
    }
    this->m_Second = Second;
}

int DateTime::GetDay() const {
    return this->m_Day;
}

int DateTime::GetMonth() const {
    return this->m_Month;
}

int DateTime::GetYear() const {
    return this->m_Year;
}

int DateTime::GetHour() const {
    return this->m_Hour;
}

int DateTime::GetMinute() const {
    return this->m_Minute;
}

int DateTime::GetSecond() const {
    return this->m_Second;
}

bool DateTime::operator <(const DateTime &other) const {
    if (this->m_Year == other.m_Year) {
        if (this->m_Month == other.m_Month) {
            if (this->m_Day == other.m_Day) {
                if (this->m_Hour == other.m_Hour) {
                    if (this->m_Minute == other.m_Minute)
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

bool DateTime::operator >(const DateTime &other) const {
    if (this->m_Year == other.m_Year) {
        if (this->m_Month == other.m_Month) {
            if (this->m_Day == other.m_Day) {
                if (this->m_Hour == other.m_Hour) {
                    if (this->m_Minute == other.m_Minute)
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

bool DateTime::operator >=(const DateTime &other) const {
    return !(*this < other);
}

bool DateTime::operator <=(const DateTime &other) const {
    return !(*this > other);
}

std::string DateTime::ToStringDate() const {
    std::ostringstream oss;
    oss << m_Day << '/' << m_Month << '/' << m_Year;
    return oss.str();
}

std::string DateTime::ToStringDateTime() const {
    std::ostringstream oss;
    oss << m_Day << '/' << m_Month << '/' << m_Year << ' '
            << std::setw(2) << std::setfill('0') << m_Hour << ':'
            << std::setw(2) << std::setfill('0') << m_Minute << ':'
            << std::setw(2) << std::setfill('0') << m_Second;
    return oss.str();
}

DateTime DateTime::FromDateTimeString(const std::string &str) {
    int month, day, year, hour, minute, second = 0; // default second to 0
    char sep1, sep2, sep3, sep4;

    std::istringstream iss(str);
    iss >> day >> sep1 >> month >> sep2 >> year >> hour >> sep3 >> minute;

    if (!iss || sep1 != '/' || sep2 != '/' || sep3 != ':') {
        throw std::invalid_argument("Invalid datetime format: " + str);
    }

    // Try to read seconds if they exist
    if (iss >> sep4 >> second) {
        if (sep4 != ':') {
            second = 0; // Invalid separator, ignore seconds
        }
    }

    return DateTime(day, month, year, hour, minute, second);
}

DateTime DateTime::FromDateString(const std::string &str) {
    int day, month, year;
    char sep1, sep2;

    std::istringstream iss(str);
    iss >> day >> sep1 >> month >> sep2 >> year;

    if (!iss || sep1 != '/' || sep2 != '/') {
        throw std::invalid_argument("Invalid date format: " + str);
    }

    return DateTime(day, month, year, 0, 0, 0); // Default time to 00:00:00
}

time_t DateTime::ToTimeT() const {
    std::tm timeinfo = {};
    timeinfo.tm_year = m_Year - 1900;
    timeinfo.tm_mon = m_Month - 1;
    timeinfo.tm_mday = m_Day;
    timeinfo.tm_hour = m_Hour;
    timeinfo.tm_min = m_Minute;
    timeinfo.tm_sec = m_Second;

    return std::mktime(&timeinfo);
}

std::vector<int> DateTime::GetValidMonths() {
    std::vector<int> months;
    for (int i = 1; i <= 12; ++i) {
        months.push_back(i);
    }
    return months;
}

std::vector<int> DateTime::GetValidDays(int month, int year) {
    std::vector<int> days;
    int maxDays = GetDaysInMonth(month, year);
    for (int i = 1; i <= maxDays; ++i) {
        days.push_back(i);
    }
    return days;
}

std::vector<int> DateTime::GetValidHours() {
    std::vector<int> hours;
    for (int i = 0; i < 24; ++i) {
        hours.push_back(i);
    }
    return hours;
}

std::vector<int> DateTime::GetValidMinutes(int interval) {
    std::vector<int> minutes;
    for (int i = 0; i < 60; i += interval) {
        minutes.push_back(i);
    }
    return minutes;
}

std::string DateTime::MonthToString(int month) {
    static const std::array<const char *, 12> monthNames = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    if (month >= 1 && month <= 12) {
        return monthNames[month - 1];
    }
    return "Invalid";
}
