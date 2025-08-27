#include "BudgetManager.hpp"
#include "Budget.hpp"
#include "BudgetRepository.hpp"
#include "BudgetService.hpp"
#include "DateTime.hpp"
#include "Logger.hpp"
#include "FileHandle.hpp"

void BudgetTest() {
    // Setup dependencies
    auto budget = std::make_shared<Budget>();
    
    auto repo = std::make_unique<BudgetRepository>("Data/Budget.json");
    auto service = std::make_unique<BudgetService>();
    
    BudgetManager manager(budget, repo, service);

    // Add income and expense

    // Show totals
    LOG_INFO("Total Income: {}", manager.GetIncomeTotal());
    LOG_INFO("Total Expense: {}", manager.GetExpenseTotal());

    // Remove bill
    // int removeId = 1;
    // manager.RemoveBillByID(removeId);

    // Print again after removal
    // LOG_INFO("After removing ID {}:", removeId);
    // LOG_INFO("Total Income: {}", manager.GetIncomeTotal());
    // LOG_INFO("Total Expense: {}", manager.GetExpenseTotal());

    // Example usage
    FilterCriteria criteria;
    criteria.SetAmountRange(100.0, 1000.0);
    criteria.SetBillType(BillType::Income);

    auto filteredBills = manager.FilterBills(criteria);
    auto sortedBills = manager.SortBills(SortCriteria::Date);
    auto filteredAndSorted = manager.FilterAndSortBills(criteria, SortCriteria::Amount);

    // Get monthly summary for 2024
    auto monthlySummary = manager.GetYearlyMonthlySummary(2025);

    // Get historical averages from 2020 to 2024
    auto historicalData = manager.GetHistoricalAverages(2020, 2025);
    std::cout << "Average monthly income: " << historicalData.averageMonthlyIncome << std::endl;

    // Access individual month data
    for (const auto& month : monthlySummary) {
        std::cout << month.GetMonthName() << ": Income=" << month.totalIncome 
                << ", Expenses=" << month.totalExpenses 
                << ", Net=" << month.netIncome << std::endl;
    }

    // Calculate total tax for all bills
    auto totalTax = manager.CalculateTotalTax();
    std::cout << "Tax owed: " << totalTax.taxOwed << " VND" << std::endl;
    std::cout << "Tax rate: " << (totalTax.taxRate * 100) << "%" << std::endl;

    // Calculate tax for 2024
    auto yearlyTax = manager.CalculateTaxForYear(2025);
    if (yearlyTax.isProfitable) {
        std::cout << "2024 Net Profit: " << yearlyTax.netProfit << " VND" << std::endl;
        std::cout << "2024 Tax Owed: " << yearlyTax.taxOwed << " VND" << std::endl;
    } else {
        std::cout << "No tax owed - business had a loss" << std::endl;
    }

    // Calculate tax for December 2024
    auto monthlyTax = manager.CalculateTaxForMonth(2025, 1);
}