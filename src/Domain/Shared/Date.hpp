#pragma once
#include<iostream>
class Date{
public:
    Date();
    Date(const int &Day,const int &Month, const int &Year);
    ~Date() = default;
    int GetDay() const;
    int GetMonth() const;
    int GetYear() const;

private:
    int m_Day;
    int m_Month;
    int m_Year;
};