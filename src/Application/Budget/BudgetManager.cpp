#include "BudgetManager.hpp"
#include "Bill.hpp"

BudgetManager::BudgetManager(std::shared_ptr<Budget> Budget, std::unique_ptr<BudgetPersistence> Persistence)
    : m_Budget(Budget), m_Persistence(std::move(Persistence)) {}

void BudgetManager::AddIncome(const int &Id, const DateTime& Date, const std::string& Message, const double &Total) {
    const BillType Type = BillType::Expense;
    auto NewBill = std::make_shared<Bill>(Id, Date, Message, Total, Type);
    m_Budget->AddIncome(NewBill);
    m_Persistence->SaveBill(NewBill);
}

void BudgetManager::AddExpense(const int &Id, const DateTime& Date, const std::string& Message, const double &Total) {
    const BillType Type = BillType::Expense;
    auto NewBill = std::make_shared<Bill>(Id, Date, Message, Total, Type);
    m_Budget->AddExpense(NewBill);
    m_Persistence->SaveBill(NewBill);
}

double BudgetManager::GetIncomeTotal() const {
    return m_Budget->GetTotalIncome();
}

double BudgetManager::GetExpenseTotal() const {
    return m_Budget->GetTotalExpenses();
}