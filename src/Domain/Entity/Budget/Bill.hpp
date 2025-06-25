#pragma once 

#include <string>
#include "DateTime.hpp"

class Bill {
private:
    const int m_BillID;
    DateTime m_Date;
    std::string m_Message;
    double m_Total;

public: 
    Bill(const int BillID, const DateTime &Date, const std::string &Message, const double Total);
};