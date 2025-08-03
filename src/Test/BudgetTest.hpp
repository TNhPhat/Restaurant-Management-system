#include "BudgetManager.hpp"
#include "Budget.hpp"
#include "BudgetRepository.hpp"
#include "BudgetService.hpp"
#include "DateTime.hpp"
#include "Logger.hpp"
#include "FileHandle.hpp"

void BudgetTest() {
    // Setup dependencies
    auto budget = std::make_shared<Budget>();
    
    auto repo = std::make_unique<BudgetRepository>("Data/Budget.json");
    auto service = std::make_unique<BudgetService>();
    
    BudgetManager manager(budget, repo, service);

    // Add income and expense
    manager.AddIncome(DateTime::Now(), "Selling Special Dish", 1500.0);
    manager.AddExpense(DateTime::Now(), "Buy kitchen equipment", 700.0);
    // Show totals
    LOG_INFO("Total Income: {}", manager.GetIncomeTotal());
    LOG_INFO("Total Expense: {}", manager.GetExpenseTotal());

    // Remove bill
    int removeId = 1;
    manager.RemoveBillByID(removeId);

    // Print again after removal
    LOG_INFO("After removing ID {}:", removeId);
    LOG_INFO("Total Income: {}", manager.GetIncomeTotal());
    LOG_INFO("Total Expense: {}", manager.GetExpenseTotal());
}