#pragma once

#include "Budget.hpp"
#include "BudgetPersistence.hpp"
#include <memory>

class BudgetManager {
private:
    std::shared_ptr<Budget> m_Budget;
    std::unique_ptr<BudgetPersistence> m_Persistence;

public:
    BudgetManager(std::shared_ptr<Budget> Budget, std::unique_ptr<BudgetPersistence> Persistence);

    void AddIncome(const int &Id, const DateTime& Date, const std::string& Message, const double &Total);
    void AddExpense(const int &Id, const DateTime& Date, const std::string& Message, const double &Total);
    double GetIncomeTotal() const;
    double GetExpenseTotal() const;
};