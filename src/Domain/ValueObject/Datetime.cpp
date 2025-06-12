#include"DateTime.hpp"
DateTime::DateTime(): m_Day(1), m_Month(1), m_Year(1), m_Hour(0), m_Minute(0) {}

DateTime::DateTime(int Day, int Month, int Year)
{
    this->m_Day = Day;
    this->m_Month = Month;
    this->m_Year = Year;
    this->m_Hour = 0;
    this->m_Minute = 0;
}

DateTime::DateTime(int Day, int Month, int Year, int Hour, int Minute)
{
    this->m_Day = Day;
    this->m_Month = Month;
    this->m_Year = Year;
    this->m_Hour = Hour;
    this->m_Minute = Minute;  
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

int DateTime::GetDay()
{
    return this->m_Day;
}

int DateTime::GetMonth()
{
    return this->m_Month;
}

int DateTime::GetYear()
{
    return this->m_Year;
}

int DateTime::GetHour()
{
    return this->m_Hour;
}

int DateTime::GetMinute()
{
    return this->m_Minute;
}
