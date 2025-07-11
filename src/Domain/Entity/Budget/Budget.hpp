#pragma once 

#include <vector>
#include <memory>
#include "Bill.hpp" 

class Budget {
private:
    double m_TotalIncome = 0.0;
    double m_TotalExpenses = 0.0;
    std::vector<std::shared_ptr<Bill>> m_Bills;

public:
    void LoadBills(const std::vector<std::shared_ptr<Bill>> &Bills);

    void AddIncome(const std::shared_ptr<Bill> &NewBill);
    void AddExpense(const std::shared_ptr<Bill> &NewBill);
    bool RemoveBillByID(const int &BillID); 

    double GetTotalIncome() const;
    double GetTotalExpenses() const;
    std::vector<std::shared_ptr<Bill>> GetAllBills() const;
};