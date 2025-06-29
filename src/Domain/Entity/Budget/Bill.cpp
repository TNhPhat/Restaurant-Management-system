#include "Bill.hpp"

Bill::Bill(const int BillID, const DateTime &Date, const std::string &Message, const double &Total, const BillType &Type) : 
    m_BillID(BillID), m_Date(Date), m_Message(Message), m_Total(Total), m_Type(Type) { }

double Bill::GetTotal() const {
    return m_Total;
}

DateTime Bill::GetDate() const {
    return m_Date;
}

std::string Bill::GetMessage() const {
    return m_Message;
}
