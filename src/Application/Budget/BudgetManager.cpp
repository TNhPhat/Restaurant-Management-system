#include "BudgetManager.hpp"
#include "Bill.hpp"
#include "Logger.hpp"

BudgetManager::BudgetManager(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository, std::unique_ptr<BudgetService> &Service)
    : m_Repository(std::move(Repository)), m_Budget(Budget), m_Service(std::move(Service)) {
    m_Budget->LoadBills(m_Repository->LoadAllBills());
}

BudgetManager::~BudgetManager() {
    m_Repository->SaveAllBills(m_Budget->GetAllBills());
}

void BudgetManager::AddIncome(const DateTime& Date, const std::string& Message, const double& Total) {
    auto bill = m_Service->GenerateBill(Date, Message, Total, BillType::Income);
    m_Budget->AddIncome(bill);
}

void BudgetManager::AddExpense(const DateTime& Date, const std::string& Message, const double& Total) {
    auto bill = m_Service->GenerateBill(Date, Message, Total, BillType::Expense);
    m_Budget->AddExpense(bill);
}

void BudgetManager::RemoveBillByID(const int &BillID) {
    if(!m_Budget->RemoveBillByID(BillID)) {
        LOG_ERROR("Bill ID: {} not found in Budget", BillID);
    }
}

double BudgetManager::GetIncomeTotal() const {
    return m_Budget->GetTotalIncome();
}

double BudgetManager::GetExpenseTotal() const {
    return m_Budget->GetTotalExpenses();
}
