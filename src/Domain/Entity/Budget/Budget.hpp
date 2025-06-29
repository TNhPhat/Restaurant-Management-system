#pragma once 

#include <vector>
#include <memory>
#include "Bill.hpp" 

class Budget {
private:
    double m_TotalIncome = 0.0;
    double m_TotalExpenses = 0.0;
    std::vector<std::shared_ptr<Bill>> m_Incomes;
    std::vector<std::shared_ptr<Bill>> m_Expenses;

public:
    void AddIncome(const std::shared_ptr<Bill> &NewBill);
    void AddExpense(const std::shared_ptr<Bill> &NewBill);
    double GetTotalIncome() const;
    double GetTotalExpenses() const;
};