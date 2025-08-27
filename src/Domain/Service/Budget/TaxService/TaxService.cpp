#include "TaxService.hpp"
#include <string>

TaxService::TaxService() 
    : m_TaxStrategy(std::make_unique<VietnameseCorporateTaxStrategy>()) {
}

TaxService::TaxService(std::unique_ptr<ITaxCalculationStrategy> strategy)
    : m_TaxStrategy(std::move(strategy)) {
}

TaxCalculationResult TaxService::CalculateTotalTax(
    const std::vector<std::shared_ptr<Bill>>& bills
) const {
    return m_TaxStrategy->CalculateTax(bills);
}

TaxCalculationResult TaxService::CalculateTaxForPeriod(
    const std::vector<std::shared_ptr<Bill>>& bills,
    const TaxPeriod& period
) const {
    auto filteredBills = FilterBillsByPeriod(bills, period);
    return m_TaxStrategy->CalculateTax(filteredBills);
}

TaxCalculationResult TaxService::CalculateTaxForYear(
    const std::vector<std::shared_ptr<Bill>>& bills,
    int year
) const {
    // Create date range for the entire year (defaults to 00:00:00 for time)
    DateTime startDate(1, 1, year);           // January 1st
    DateTime endDate(31, 12, year, 23, 59, 59);  // December 31st, end of day
    TaxPeriod yearPeriod(startDate, endDate, "Year " + std::to_string(year));
    
    return CalculateTaxForPeriod(bills, yearPeriod);
}

TaxCalculationResult TaxService::CalculateTaxForMonth(
    const std::vector<std::shared_ptr<Bill>>& bills,
    int year,
    int month
) const {
    // Calculate last day of month (more accurate calculation)
    int lastDay = 31; // Default
    if (month == 2) {
        // Simple leap year check
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        lastDay = isLeapYear ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        lastDay = 30;
    }
    
    DateTime startDate(1, month, year);                    // First day (defaults to 00:00:00)
    DateTime endDate(lastDay, month, year, 23, 59, 59);   // Last day, end of day
    TaxPeriod monthPeriod(startDate, endDate, 
        "Month " + std::to_string(month) + "/" + std::to_string(year));
    
    return CalculateTaxForPeriod(bills, monthPeriod);
}

double TaxService::GetCurrentTaxRate() const {
    return m_TaxStrategy->GetTaxRate();
}

std::vector<std::shared_ptr<Bill>> TaxService::FilterBillsByPeriod(
    const std::vector<std::shared_ptr<Bill>>& bills,
    const TaxPeriod& period
) const {
    std::vector<std::shared_ptr<Bill>> filteredBills;
    
    for (const auto& bill : bills) {
        if (IsBillInPeriod(bill, period)) {
            filteredBills.push_back(bill);
        }
    }
    
    return filteredBills;
}

bool TaxService::IsBillInPeriod(const std::shared_ptr<Bill>& bill, const TaxPeriod& period) const {
    DateTime billDate = bill->GetDate();
    return billDate >= period.startDate && billDate <= period.endDate;
}