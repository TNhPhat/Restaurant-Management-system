#pragma once

#include "Budget.hpp"
#include "BudgetRepository.hpp"
#include "BudgetService.hpp"
#include "FilterCriteria.hpp"
#include "SortStrategy.hpp"
#include "StatisticsStrategy.hpp"
#include "TaxService.hpp"
#include <memory>

class BudgetManager {
private:
    std::shared_ptr<Budget> m_Budget;
    std::unique_ptr<BudgetRepository> m_Repository;
    std::unique_ptr<BudgetService> m_Service;

public:
    BudgetManager(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository, std::unique_ptr<BudgetService> &Service);
    ~BudgetManager();
    
    // Original functionality
    void AddIncome(const DateTime& Date, const std::string& Message, const double& Total);
    void AddExpense(const DateTime& Date, const std::string& Message, const double& Total);
    void RemoveBillByID(const int& BillID);
    double GetIncomeTotal() const;
    double GetExpenseTotal() const;
    
    // New filtering functionality
    std::vector<std::shared_ptr<Bill>> FilterBills(const FilterCriteria& criteria) const;
    
    // New sorting functionality
    std::vector<std::shared_ptr<Bill>> SortBills(SortCriteria criteria) const;
    
    // Find bill by ID
    std::shared_ptr<Bill> FindBillByID(int billID) const;
    
    // Combined filter and sort
    std::vector<std::shared_ptr<Bill>> FilterAndSortBills(
        const FilterCriteria& criteria, 
        SortCriteria sortCriteria
    ) const;
    
    // Get all bills (for service operations)
    std::vector<std::shared_ptr<Bill>> GetAllBills() const;
    
    // Statistics functionality
    std::vector<MonthlySummary> GetYearlyMonthlySummary(int year) const;
    
    HistoricalAveragesCalculator::HistoricalData GetHistoricalAverages(
        int startYear, 
        int endYear
    ) const;
    
    // Tax calculation functionality
    TaxCalculationResult CalculateTotalTax() const;
    TaxCalculationResult CalculateTaxForYear(int year) const;
    TaxCalculationResult CalculateTaxForMonth(int year, int month) const;
    double GetCurrentTaxRate() const;
};