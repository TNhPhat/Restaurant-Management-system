#include "BudgetService.hpp"
#include "IDManager.hpp"
#include <algorithm>

BudgetService::BudgetService()
    : m_FilterStrategy(std::make_unique<BillFilterStrategy>()),
      m_TaxService(std::make_unique<TaxService>()) {
}

std::shared_ptr<Bill> BudgetService::GenerateBill(const DateTime &Date, const std::string &Message,
                                                  const double &Price, const BillType &Type) {
    int newBillID = IDManager::GetInstance().GetNextID("Bill");
    return std::make_shared<Bill>(newBillID, Date, Message, Price, Type);
}

std::vector<std::shared_ptr<Bill> > BudgetService::FilterBills(
    const std::vector<std::shared_ptr<Bill> > &bills,
    const FilterCriteria &criteria
) const {
    return m_FilterStrategy->Filter(bills, criteria);
}

std::vector<std::shared_ptr<Bill> > BudgetService::SortBills(
    std::vector<std::shared_ptr<Bill> > bills, // Copy for sorting
    SortCriteria criteria
) const {
    auto sortStrategy = SortStrategyFactory::CreateStrategy(criteria);
    sortStrategy->Sort(bills);
    return bills;
}

std::shared_ptr<Bill> BudgetService::FindBillByID(
    const std::vector<std::shared_ptr<Bill> > &bills,
    int billID
) const {
    auto it = std::find_if(bills.begin(), bills.end(),
                           [billID](const std::shared_ptr<Bill> &bill) {
                               return bill->GetID() == billID;
                           });

    return (it != bills.end()) ? *it : nullptr;
}

std::vector<MonthlySummary> BudgetService::GetYearlyMonthlySummary(
    const std::vector<std::shared_ptr<Bill> > &bills,
    int year
) const {
    YearlyMonthlySummaryStrategy strategy(year);
    return strategy.CalculateStatistics(bills);
}

HistoricalAveragesCalculator::HistoricalData BudgetService::GetHistoricalAverages(
    const std::vector<std::shared_ptr<Bill> > &bills,
    int startYear,
    int endYear
) const {
    std::vector<MonthlySummary> allSummaries;

    // Collect monthly summaries for all years in range
    for (int year = startYear; year <= endYear; ++year) {
        auto yearSummaries = GetYearlyMonthlySummary(bills, year);
        allSummaries.insert(allSummaries.end(), yearSummaries.begin(), yearSummaries.end());
    }

    return HistoricalAveragesCalculator::CalculateHistoricalAverages(allSummaries);
}

TaxCalculationResult BudgetService::CalculateTotalTax(
    const std::vector<std::shared_ptr<Bill> > &bills
) const {
    return m_TaxService->CalculateTotalTax(bills);
}

TaxCalculationResult BudgetService::CalculateTaxForYear(
    const std::vector<std::shared_ptr<Bill> > &bills,
    int year
) const {
    return m_TaxService->CalculateTaxForYear(bills, year);
}

TaxCalculationResult BudgetService::CalculateTaxForMonth(
    const std::vector<std::shared_ptr<Bill> > &bills,
    int year,
    int month
) const {
    return m_TaxService->CalculateTaxForMonth(bills, year, month);
}

double BudgetService::GetCurrentTaxRate() const {
    return m_TaxService->GetCurrentTaxRate();
}

double BudgetService::GetIncomeInYear(const std::vector<std::shared_ptr<Bill> > &bills, int get_year) {
    double totalIncome = 0.0;
    for (const auto &bill: bills) {
        if (bill->GetType() == BillType::Income &&
            bill->GetDate().GetYear() == get_year) {
            totalIncome += bill->GetTotal();
        }
    }
    return totalIncome;
}

double BudgetService::GetExpenseInYear(const std::vector<std::shared_ptr<Bill> > &bills, int get_year) {
    double totalExpense = 0.0;
    for (const auto &bill: bills) {
        if (bill->GetType() == BillType::Expense &&
            bill->GetDate().GetYear() == get_year) {
            totalExpense += bill->GetTotal();
        }
    }
    return totalExpense;
}

double BudgetService::GetIncomeInMonth(const std::vector<std::shared_ptr<Bill> > &bills, int year, int month) {
    double totalIncome = 0.0;
    for (const auto &bill: bills) {
        if (bill->GetType() == BillType::Income &&
            bill->GetDate().GetYear() == year &&
            bill->GetDate().GetMonth() == month) {
            totalIncome += bill->GetTotal();
        }
    }
    return totalIncome;
}

double BudgetService::GetExpenseInMonth(const std::vector<std::shared_ptr<Bill> > &bills, int year, int month) {
    double totalExpense = 0.0;
    for (const auto &bill: bills) {
        if (bill->GetType() == BillType::Expense &&
            bill->GetDate().GetYear() == year &&
            bill->GetDate().GetMonth() == month) {
            totalExpense += bill->GetTotal();
        }
    }
    return totalExpense;
}
