#include "StatisticsStrategy.hpp"
#include <algorithm>
#include <map>

std::vector<MonthlySummary> YearlyMonthlySummaryStrategy::CalculateStatistics(
    const std::vector<std::shared_ptr<Bill>>& bills
) const {
    std::map<int, MonthlySummary> monthlyData;
    
    // Initialize all 12 months with proper constructor arguments
    for (int month = 1; month <= 12; ++month) {
        monthlyData[month] = MonthlySummary(m_TargetYear, month);
    }
    
    // Process bills for the target year
    for (const auto& bill : bills) {
        if (bill->GetDate().GetYear() == m_TargetYear) {
            int month = bill->GetDate().GetMonth();
            
            if (month >= 1 && month <= 12) {
                if (bill->GetType() == BillType::Income) {
                    monthlyData[month].AddIncome(bill->GetTotal());
                } else if (bill->GetType() == BillType::Expense) {
                    monthlyData[month].AddExpense(bill->GetTotal());
                }
            }
        }
    }
    
    // Convert map to vector
    std::vector<MonthlySummary> result;
    for (int month = 1; month <= 12; ++month) {
        result.push_back(monthlyData[month]);
    }
    
    return result;
}

HistoricalAveragesCalculator::HistoricalData HistoricalAveragesCalculator::CalculateHistoricalAverages(
    const std::vector<MonthlySummary>& monthlySummaries
) {
    HistoricalData result;
    
    if (monthlySummaries.empty()) {
        return result;
    }
    
    double totalIncome = 0.0;
    double totalExpenses = 0.0;
    double totalNet = 0.0;
    int validMonths = 0;
    
    for (const auto& summary : monthlySummaries) {
        // Only count months that have some activity
        if (summary.incomeCount > 0 || summary.expenseCount > 0) {
            totalIncome += summary.totalIncome;
            totalExpenses += summary.totalExpenses;
            totalNet += summary.netIncome;
            validMonths++;
        }
    }
    
    if (validMonths > 0) {
        result.averageMonthlyIncome = totalIncome / validMonths;
        result.averageMonthlyExpense = totalExpenses / validMonths;
        result.averageMonthlyNet = totalNet / validMonths;
        result.totalMonths = validMonths;
    }
    
    return result;
}