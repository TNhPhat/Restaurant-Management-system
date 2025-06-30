#pragma once 

#include <string>
#include "DateTime.hpp"
#include "BillType.hpp"

class Bill {
private:
    const int m_BillID;
    DateTime m_Date;
    std::string m_Message;
    double m_Total;
    BillType m_Type;

public: 
    Bill(const int &BillID, const DateTime &Date, const std::string &Message, const double &Total, const BillType &Type);
    
    int GetID() const;
    double GetTotal() const;
    DateTime GetDate() const;
    std::string GetMessage() const;
    BillType GetType() const;
};