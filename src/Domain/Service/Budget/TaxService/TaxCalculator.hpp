#pragma once

#include <vector>
#include <memory>
#include "Bill.hpp"

struct TaxCalculationResult {
    double totalIncome;
    double totalExpenses;
    double netProfit;
    double taxableAmount; // Only positive net profit is taxable
    double taxRate;
    double taxOwed;
    bool isProfitable;
    
    TaxCalculationResult() 
        : totalIncome(0.0), totalExpenses(0.0), netProfit(0.0), 
          taxableAmount(0.0), taxRate(0.0), taxOwed(0.0), isProfitable(false) {}
};

class ITaxCalculationStrategy {
public:
    virtual ~ITaxCalculationStrategy() = default;
    virtual TaxCalculationResult CalculateTax(const std::vector<std::shared_ptr<Bill>>& bills) const = 0;
    virtual double GetTaxRate() const = 0;
};

class VietnameseCorporateTaxStrategy : public ITaxCalculationStrategy {
private:
    static constexpr double VIETNAM_CORPORATE_TAX_RATE = 0.20; // 20%
    
public:
    TaxCalculationResult CalculateTax(const std::vector<std::shared_ptr<Bill>>& bills) const override;
    
    double GetTaxRate() const override {
        return VIETNAM_CORPORATE_TAX_RATE;
    }
    
private:
    void CalculateTotals(const std::vector<std::shared_ptr<Bill>>& bills, double& totalIncome, double& totalExpenses) const;
};