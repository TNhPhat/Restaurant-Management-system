#include "BudgetManager.hpp"
#include "Bill.hpp"
#include "Logger.hpp"

BudgetManager::BudgetManager(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository,
                             std::unique_ptr<BudgetService> &Service)
    : m_Repository(std::move(Repository)), m_Budget(Budget), m_Service(std::move(Service)) {
    m_Budget->LoadBills(m_Repository->LoadAllBills());
}

BudgetManager::~BudgetManager() {
    m_Repository->SaveAllBills(m_Budget->GetAllBills());
}

// Original functionality
void BudgetManager::AddIncome(const DateTime &Date, const std::string &Message, const double &Total) {
    auto bill = m_Service->GenerateBill(Date, Message, Total, BillType::Income);
    m_Budget->AddIncome(bill);
}

void BudgetManager::AddExpense(const DateTime &Date, const std::string &Message, const double &Total) {
    auto bill = m_Service->GenerateBill(Date, Message, Total, BillType::Expense);
    m_Budget->AddExpense(bill);
}

void BudgetManager::RemoveBillByID(const int &BillID) {
    if (!m_Budget->RemoveBillByID(BillID)) {
        LOG_ERROR("Bill ID: {} not found in Budget", BillID);
    }
}

double BudgetManager::GetIncomeTotal() const {
    return m_Budget->GetTotalIncome();
}

double BudgetManager::GetExpenseTotal() const {
    return m_Budget->GetTotalExpenses();
}

// New functionality
std::vector<std::shared_ptr<Bill> > BudgetManager::FilterBills(const FilterCriteria &criteria) const {
    return m_Service->FilterBills(m_Budget->GetAllBills(), criteria);
}

std::vector<std::shared_ptr<Bill> > BudgetManager::SortBills(SortCriteria criteria) const {
    return m_Service->SortBills(m_Budget->GetAllBills(), criteria);
}

std::shared_ptr<Bill> BudgetManager::FindBillByID(int billID) const {
    return m_Service->FindBillByID(m_Budget->GetAllBills(), billID);
}

std::vector<std::shared_ptr<Bill> > BudgetManager::FilterAndSortBills(
    const FilterCriteria &criteria,
    SortCriteria sortCriteria
) const {
    auto filteredBills = m_Service->FilterBills(m_Budget->GetAllBills(), criteria);
    return m_Service->SortBills(filteredBills, sortCriteria);
}

std::vector<std::shared_ptr<Bill> > BudgetManager::GetAllBills() const {
    return m_Budget->GetAllBills();
}

std::vector<MonthlySummary> BudgetManager::GetYearlyMonthlySummary(int year) const {
    return m_Service->GetYearlyMonthlySummary(m_Budget->GetAllBills(), year);
}

HistoricalAveragesCalculator::HistoricalData BudgetManager::GetHistoricalAverages(
    int startYear,
    int endYear
) const {
    return m_Service->GetHistoricalAverages(m_Budget->GetAllBills(), startYear, endYear);
}

TaxCalculationResult BudgetManager::CalculateTotalTax() const {
    return m_Service->CalculateTotalTax(m_Budget->GetAllBills());
}

TaxCalculationResult BudgetManager::CalculateTaxForYear(int year) const {
    return m_Service->CalculateTaxForYear(m_Budget->GetAllBills(), year);
}

TaxCalculationResult BudgetManager::CalculateTaxForMonth(int year, int month) const {
    return m_Service->CalculateTaxForMonth(m_Budget->GetAllBills(), year, month);
}

double BudgetManager::GetCurrentTaxRate() const {
    return m_Service->GetCurrentTaxRate();
}
