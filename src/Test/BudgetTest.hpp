#pragma once
#include "FileHandle.hpp"
#include "BudgetPersistence.hpp"
#include "Budget.hpp"
#include "BudgetManager.hpp"


void BudgetTest() {
    JsonHandle* JsonHandler = new JsonHandle();

    Budget budget;
    BudgetPersistence BP("Data/Budget.json", JsonHandler);
    BillType Type = BillType::Expense;
    BP.SaveBill(std::make_shared<Bill>(69, DateTime::FromDateTimeString("30/6/2024 22:00:00"), "HeheTest", 100.69, Type));
    BP.RemoveBillByID(10);
}