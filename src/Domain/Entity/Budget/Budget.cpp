#include "Budget.hpp"

void Budget::AddIncome(const std::shared_ptr<Bill> &NewBill) {
    m_TotalIncome += NewBill->GetTotal();
    m_Incomes.push_back(NewBill);
}

void Budget::AddExpense(const std::shared_ptr<Bill> &NewBill) {
    m_TotalExpenses += NewBill->GetTotal();
    m_Expenses.push_back(NewBill);
}

double Budget::GetTotalIncome() const {
    return m_TotalIncome;
}

double Budget::GetTotalExpenses() const {
    return m_TotalExpenses;
}
