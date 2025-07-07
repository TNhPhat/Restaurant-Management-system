#pragma once

#include <vector>
#include <memory>
#include "Bill.hpp"
#include "FileHandle.hpp"

class BudgetRepository {
private:
    JsonHandle* m_FileHandler;
    std::string m_FilePath;

public:
    BudgetRepository(const std::string& FilePath, JsonHandle* FileHandler);

    void SaveBill(const std::shared_ptr<Bill>& Bill);
    std::vector<std::shared_ptr<Bill>> LoadAllBills();
    bool RemoveBillByID(const int &BillID);
};
