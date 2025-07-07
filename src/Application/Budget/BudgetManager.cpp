#include "BudgetManager.hpp"
#include "Bill.hpp"
#include "Logger.hpp"

std::unique_ptr<BudgetManager> BudgetManager::s_Instance = nullptr;

BudgetManager::BudgetManager(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository, std::unique_ptr<BudgetService> &Service)
    : m_Repository(std::move(Repository)), m_Budget(Budget), m_Service(std::move(Service)) {
    m_Budget->LoadBills(m_Repository->LoadAllBills());
}

void BudgetManager::Init(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository, std::unique_ptr<BudgetService> &Service) {
    if (s_Instance) 
        LOG_ERROR("IDManager already initialized!");
    s_Instance = std::unique_ptr<BudgetManager>(new BudgetManager(Budget, Repository, Service));
}

BudgetManager &BudgetManager::GetInstance() {
    if (!s_Instance) {
        LOG_CRITICAL("BudgetManager not initialized");
        throw std::runtime_error("BudgetManager not initialized");
    }
    return *s_Instance;
}

void BudgetManager::AddIncome(const DateTime& Date, const std::string& Message, const double& Total) {
    auto bill = m_Service->GenerateBill(Date, Message, Total, BillType::Income);
    m_Budget->AddIncome(bill);
    m_Repository->SaveBill(bill);
}

void BudgetManager::AddExpense(const DateTime& Date, const std::string& Message, const double& Total) {
    auto bill = m_Service->GenerateBill(Date, Message, Total, BillType::Expense);
    m_Budget->AddExpense(bill);
    m_Repository->SaveBill(bill);
}

void BudgetManager::RemoveBillByID(const int &BillID) {
    if(!m_Repository->RemoveBillByID(BillID)) {
        LOG_ERROR("Bill ID: {} not found in repository", BillID);
    } else if(!m_Budget->RemoveBillByID(BillID)) {
        LOG_ERROR("Bill ID: {} not found in Budget", BillID);
    }
}

double BudgetManager::GetIncomeTotal() const {
    return m_Budget->GetTotalIncome();
}

double BudgetManager::GetExpenseTotal() const {
    return m_Budget->GetTotalExpenses();
}
