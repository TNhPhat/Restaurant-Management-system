#include "Budget.hpp"

void Budget::AddIncome(const double &Amount) {
    m_TotalIncome += Amount;
}

void Budget::AddExpense(const double &Amount) {
    m_TotalExpenses += Amount;
}
