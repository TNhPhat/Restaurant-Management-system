#pragma once 

#include <vector>
#include "Bill.hpp" 

class Budget {
private:
    double m_TotalIncome;
    double m_TotalExpenses;
    
public:
    void AddIncome(const double &Amount);
    void AddExpense(const double &Amount);
};