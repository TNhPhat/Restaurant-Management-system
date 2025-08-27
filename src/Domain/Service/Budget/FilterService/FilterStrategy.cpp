#include "FilterStrategy.hpp"
#include <algorithm>

std::vector<std::shared_ptr<Bill> > BillFilterStrategy::Filter(
    const std::vector<std::shared_ptr<Bill> > &bills,
    const FilterCriteria &criteria
) const {
    std::vector<std::shared_ptr<Bill> > filteredBills;

    for (const auto &bill: bills) {
        bool passesFilter = true;

        if (criteria.GetAmountRange().has_value()) {
            if (!PassesAmountFilter(bill, criteria.GetAmountRange().value())) {
                passesFilter = false;
            }
        }

        if (criteria.GetDateRange().has_value() && passesFilter) {
            if (!PassesDateFilter(bill, criteria.GetDateRange().value())) {
                passesFilter = false;
            }
        }

        if (passesFilter) {
            if (bill->GetType() == BillType::Income && !criteria.GetShowIncome()) {
                passesFilter = false;
            } else if (bill->GetType() == BillType::Expense && !criteria.GetShowExpense()) {
                passesFilter = false;
            }
        }

        if (passesFilter) {
            filteredBills.push_back(bill);
        }
    }

    return filteredBills;
}

bool BillFilterStrategy::PassesAmountFilter(const std::shared_ptr<Bill> &bill, const AmountRange &range) const {
    double amount = bill->GetTotal();
    return amount >= range.minAmount && amount <= range.maxAmount;
}

bool BillFilterStrategy::PassesDateFilter(const std::shared_ptr<Bill> &bill, const DateRange &range) const {
    DateTime billDate = bill->GetDate();
    return billDate >= range.startDate && billDate <= range.endDate;
}

bool BillFilterStrategy::PassesBillTypeFilter(const std::shared_ptr<Bill> &bill, BillType type) const {
    return bill->GetType() == type;
}
