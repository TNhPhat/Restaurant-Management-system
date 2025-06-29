#include "BudgetApp.hpp"
#include "Bill.hpp"

BudgetAppService::BudgetAppService(std::shared_ptr<Budget> budget, std::unique_ptr<BudgetPersistence> persistence)
    : m_Budget(budget), m_Persistence(std::move(persistence)) {}

void BudgetAppService::AddIncome(int id, const DateTime& date, const std::string& message, double total) {
    auto bill = std::make_shared<Bill>(id, date, message, total);
    m_Budget->AddIncome(bill);
    m_Persistence->SaveBill(bill, true);
}

void BudgetAppService::AddExpense(int id, const DateTime& date, const std::string& message, double total) {
    auto bill = std::make_shared<Bill>(id, date, message, total);
    m_Budget->AddExpense(bill);
    m_Persistence->SaveBill(bill, false);
}

void BudgetAppService::Save() {
    m_Persistence->SaveFile(); 
}

double BudgetAppService::GetIncomeTotal() const {
    return m_Budget->GetTotalIncome();
}

double BudgetAppService::GetExpenseTotal() const {
    return m_Budget->GetTotalExpenses();
}
