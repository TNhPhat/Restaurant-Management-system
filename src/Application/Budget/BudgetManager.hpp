#pragma once

#include "Budget.hpp"
#include "BudgetRepository.hpp"
#include "BudgetService.hpp"
#include <memory>

class BudgetManager {
private:
    static std::unique_ptr<BudgetManager> s_Instance;

    std::shared_ptr<Budget> m_Budget;
    std::unique_ptr<BudgetRepository> m_Repository;
    std::unique_ptr<BudgetService> m_Service;

    BudgetManager(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository, std::unique_ptr<BudgetService> &Service);

public:
    static void Init(std::shared_ptr<Budget> &Budget, std::unique_ptr<BudgetRepository> &Repository, std::unique_ptr<BudgetService> &Service);
    static BudgetManager& GetInstance();

    void AddIncome(const DateTime& Date, const std::string& Message, const double& Total);
    void AddExpense(const DateTime& Date, const std::string& Message, const double& Total);

    void RemoveBillByID(const int& BillID); 

    double GetIncomeTotal() const;
    double GetExpenseTotal() const;
};
