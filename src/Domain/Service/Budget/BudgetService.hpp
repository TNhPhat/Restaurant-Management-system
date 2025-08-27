#pragma once

#include <string>
#include <memory>
#include <vector>
#include "Bill.hpp"
#include "DateTime.hpp"
#include "BillType.hpp"
#include "FilterCriteria.hpp"
#include "FilterStrategy.hpp"
#include "SortStrategy.hpp"
#include "StatisticsStrategy.hpp"
#include "TaxService.hpp"

class BudgetService {
private:
    std::unique_ptr<IFilterStrategy> m_FilterStrategy;
    std::unique_ptr<TaxService> m_TaxService;

public:
    BudgetService();

    // Original functionality
    std::shared_ptr<Bill> GenerateBill(const DateTime &Date, const std::string &Message,
                                       const double &Price, const BillType &Type);

    // New filtering functionality
    std::vector<std::shared_ptr<Bill> > FilterBills(
        const std::vector<std::shared_ptr<Bill> > &bills,
        const FilterCriteria &criteria
    ) const;

    // New sorting functionality
    std::vector<std::shared_ptr<Bill> > SortBills(
        std::vector<std::shared_ptr<Bill> > bills, // Copy for sorting
        SortCriteria criteria
    ) const;

    // Find bill by ID
    std::shared_ptr<Bill> FindBillByID(
        const std::vector<std::shared_ptr<Bill> > &bills,
        int billID
    ) const;

    // Statistics functionality
    std::vector<MonthlySummary> GetYearlyMonthlySummary(
        const std::vector<std::shared_ptr<Bill> > &bills,
        int year
    ) const;

    HistoricalAveragesCalculator::HistoricalData GetHistoricalAverages(
        const std::vector<std::shared_ptr<Bill> > &bills,
        int startYear,
        int endYear
    ) const;

    // Tax calculation functionality
    TaxCalculationResult CalculateTotalTax(
        const std::vector<std::shared_ptr<Bill> > &bills
    ) const;

    TaxCalculationResult CalculateTaxForYear(
        const std::vector<std::shared_ptr<Bill> > &bills,
        int year
    ) const;

    TaxCalculationResult CalculateTaxForMonth(
        const std::vector<std::shared_ptr<Bill> > &bills,
        int year,
        int month
    ) const;

    double GetCurrentTaxRate() const;

    double GetIncomeInYear(const std::vector<std::shared_ptr<Bill> > &bills, int get_year);

    double GetExpenseInYear(const std::vector<std::shared_ptr<Bill> > &bills, int get_year);

    static double GetIncomeInMonth(const std::vector<std::shared_ptr<Bill> > &bills, int year, int month);

    static double GetExpenseInMonth(const std::vector<std::shared_ptr<Bill> > &bills, int year, int month);
};
