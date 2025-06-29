#pragma once

#include "Budget.hpp"
#include "BudgetPersistence.hpp"
#include <memory>

class BudgetApp {
private:
    std::shared_ptr<Budget> m_Budget;
    std::unique_ptr<BudgetPersistence> m_Persistence;

public:
    BudgetAppService(std::shared_ptr<Budget> budget, std::unique_ptr<BudgetPersistence> persistence);

    void AddIncome(int id, const DateTime& date, const std::string& message, double total);
    void AddExpense(int id, const DateTime& date, const std::string& message, double total);
    void Save();
    double GetIncomeTotal() const;
    double GetExpenseTotal() const;
};
