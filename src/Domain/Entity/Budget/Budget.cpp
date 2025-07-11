#include "Budget.hpp"

void Budget::LoadBills(const std::vector<std::shared_ptr<Bill>> &Bills) {
    for(auto &NewBill : Bills) {
        if(NewBill->GetType() == BillType::Expense) 
            AddExpense(NewBill);
        if(NewBill->GetType() == BillType::Income) 
            AddIncome(NewBill);
    }
}

void Budget::AddIncome(const std::shared_ptr<Bill> &NewBill) {
    m_TotalIncome += NewBill->GetTotal();
    m_Bills.push_back(NewBill);
}

void Budget::AddExpense(const std::shared_ptr<Bill> &NewBill) {
    m_TotalExpenses += NewBill->GetTotal();
    m_Bills.push_back(NewBill);
}

bool Budget::RemoveBillByID(const int& BillID) {
    auto removeFrom = [](std::vector<std::shared_ptr<Bill>>& list, double& total, int BillID) {
        for (auto it = list.begin(); it != list.end(); ++it) {
            if ((*it)->GetID() == BillID) {
                total -= (*it)->GetTotal();
                list.erase(it);
                return true;
            }
        }
        return false;
    };

    if (removeFrom(m_Bills, m_TotalIncome, BillID)) return true; 
    return false;
}

double Budget::GetTotalIncome() const {
    return m_TotalIncome;
}

double Budget::GetTotalExpenses() const {
    return m_TotalExpenses;
}

std::vector<std::shared_ptr<Bill>> Budget::GetAllBills() const {
    return m_Bills;
}
