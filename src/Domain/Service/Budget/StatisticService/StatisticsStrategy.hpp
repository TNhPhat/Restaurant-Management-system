#pragma once

#include <vector>
#include <memory>
#include <map>
#include "Bill.hpp"
#include "MonthlySummary.hpp"

class IStatisticsStrategy {
public:
    virtual ~IStatisticsStrategy() = default;
    virtual std::vector<MonthlySummary> CalculateStatistics(
        const std::vector<std::shared_ptr<Bill>>& bills
    ) const = 0;
};

// Concrete strategy for yearly monthly summaries
class YearlyMonthlySummaryStrategy : public IStatisticsStrategy {
private:
    int m_TargetYear;
    
public:
    explicit YearlyMonthlySummaryStrategy(int year) : m_TargetYear(year) {}
    
    std::vector<MonthlySummary> CalculateStatistics(
        const std::vector<std::shared_ptr<Bill>>& bills
    ) const override;

};

class HistoricalAveragesCalculator {
public:
    struct HistoricalData {
        double averageMonthlyIncome;
        double averageMonthlyExpense;
        double averageMonthlyNet;
        int totalMonths;
        
        HistoricalData() : averageMonthlyIncome(0.0), averageMonthlyExpense(0.0), 
                          averageMonthlyNet(0.0), totalMonths(0) {}
    };
    
    static HistoricalData CalculateHistoricalAverages(
        const std::vector<MonthlySummary>& monthlySummaries
    );
};