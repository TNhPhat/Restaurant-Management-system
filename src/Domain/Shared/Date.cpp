#include"Date.hpp"
Date::Date() : m_Day(1),m_Month(1),m_Year(1) {}

Date::Date(const int &Day, const int &Month, const int &Year) :
    m_Day(Day), m_Month(Month), m_Year(Year) {}

int Date::GetDay() const{
    return this->m_Day;
}

int Date::GetMonth() const{
    return this->m_Month;
}

int Date::GetYear() const{
    return this->m_Year;
}

