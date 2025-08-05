#include "TaxCalculator.hpp"

TaxCalculationResult VietnameseCorporateTaxStrategy::CalculateTax(
    const std::vector<std::shared_ptr<Bill>>& bills
) const {
    TaxCalculationResult result;
    
    // Calculate totals
    CalculateTotals(bills, result.totalIncome, result.totalExpenses);
    
    // Calculate net profit
    result.netProfit = result.totalIncome - result.totalExpenses;
    
    // Set tax rate
    result.taxRate = VIETNAM_CORPORATE_TAX_RATE;
    
    // Check if profitable
    result.isProfitable = result.netProfit > 0.0;
    
    if (result.isProfitable) {
        // Only tax positive net profit
        result.taxableAmount = result.netProfit;
        result.taxOwed = result.taxableAmount * result.taxRate;
    } else {
        // No tax owed if not profitable
        result.taxableAmount = 0.0;
        result.taxOwed = 0.0;
    }
    
    return result;
}

void VietnameseCorporateTaxStrategy::CalculateTotals(
    const std::vector<std::shared_ptr<Bill>>& bills,
    double& totalIncome,
    double& totalExpenses
) const {
    totalIncome = 0.0;
    totalExpenses = 0.0;
    
    for (const auto& bill : bills) {
        if (bill->GetType() == BillType::Income) {
            totalIncome += bill->GetTotal();
        } else if (bill->GetType() == BillType::Expense) {
            totalExpenses += bill->GetTotal();
        }
    }
}