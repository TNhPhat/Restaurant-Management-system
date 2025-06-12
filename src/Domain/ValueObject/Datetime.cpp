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

void DateTime::setDay(int Day)
{
    this->m_Day = Day;
}

void DateTime::setMonth(int Month)
{
    this->m_Month = Month;
}

void DateTime::setYear(int year)
{
    this->m_Year = Year;
}

void DateTime::setHour(int Hour)
{
    this->m_Hour = Hour;
}

void DateTime::setMinute(int minute)
{
    this->m_Minute = Minute;
}

int DateTime::getDay()
{
    return this->m_Day;
}

int DateTime::getMonth()
{
    return this->m_Month;
}

int DateTime::getYear()
{
    return this->m_Year;
}

int DateTime::getHour()
{
    return this->m_Hour;
}

int DateTime::getMinute()
{
    return this->m_Minute;
}
