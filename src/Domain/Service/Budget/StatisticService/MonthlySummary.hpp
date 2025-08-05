#pragma once

#include <string>

struct MonthlySummary {
    int year;
    int month;
    double totalIncome;
    double totalExpenses;
    double netIncome; 
    int incomeCount;
    int expenseCount;
    
    MonthlySummary() : year(0), month(0), totalIncome(0.0), totalExpenses(0.0), 
                      netIncome(0.0), incomeCount(0), expenseCount(0) {}
    
    MonthlySummary(int y, int m) 
        : year(y), month(m), totalIncome(0.0), totalExpenses(0.0), 
          netIncome(0.0), incomeCount(0), expenseCount(0) {}
    
    void AddIncome(double amount) {
        totalIncome += amount;
        incomeCount++;
        UpdateNetIncome();
    }
    
    void AddExpense(double amount) {
        totalExpenses += amount;
        expenseCount++;
        UpdateNetIncome();
    }
    
    double GetAverageIncome() const {
        return incomeCount > 0 ? totalIncome / incomeCount : 0.0;
    }
    
    double GetAverageExpense() const {
        return expenseCount > 0 ? totalExpenses / expenseCount : 0.0;
    }
    
    std::string GetMonthName() const {
        const std::string months[] = {
            "January", "February", "March", "April", "May", "June",
            "July", "August", "September", "October", "November", "December"
        };
        return (month >= 1 && month <= 12) ? months[month - 1] : "Unknown";
    }
    
private:
    void UpdateNetIncome() {
        netIncome = totalIncome - totalExpenses;
    }
};