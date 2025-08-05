#pragma once

#include <memory>
#include <vector>
#include "TaxCalculator.hpp"
#include "Bill.hpp"
#include "DateTime.hpp"

// Tax period structure
struct TaxPeriod {
    DateTime startDate;
    DateTime endDate;
    std::string description;
    
    TaxPeriod(const DateTime& start, const DateTime& end, const std::string& desc)
        : startDate(start), endDate(end), description(desc) {}
};

class TaxService {
private:
    std::unique_ptr<ITaxCalculationStrategy> m_TaxStrategy;
    
public:
    TaxService();
    explicit TaxService(std::unique_ptr<ITaxCalculationStrategy> strategy);
    
    // Calculate tax for all bills
    TaxCalculationResult CalculateTotalTax(
        const std::vector<std::shared_ptr<Bill>>& bills
    ) const;
    
    // Calculate tax for a specific period
    TaxCalculationResult CalculateTaxForPeriod(
        const std::vector<std::shared_ptr<Bill>>& bills,
        const TaxPeriod& period
    ) const;
    
    // Calculate tax for a specific year
    TaxCalculationResult CalculateTaxForYear(
        const std::vector<std::shared_ptr<Bill>>& bills,
        int year
    ) const;
    
    // Calculate tax for a specific month
    TaxCalculationResult CalculateTaxForMonth(
        const std::vector<std::shared_ptr<Bill>>& bills,
        int year,
        int month
    ) const;
    
    // Get current tax rate
    double GetCurrentTaxRate() const;
    
private:
    std::vector<std::shared_ptr<Bill>> FilterBillsByPeriod(
        const std::vector<std::shared_ptr<Bill>>& bills,
        const TaxPeriod& period
    ) const;
    
    bool IsBillInPeriod(const std::shared_ptr<Bill>& bill, const TaxPeriod& period) const;
};