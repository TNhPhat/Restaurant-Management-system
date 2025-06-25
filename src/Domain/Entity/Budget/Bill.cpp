#include "Bill.hpp"

Bill::Bill(const int BillID, const DateTime &Date, const std::string &Message, const double Total) : 
    m_BillID(BillID), m_Date(Date), m_Message(Message), m_Total(Total) { }