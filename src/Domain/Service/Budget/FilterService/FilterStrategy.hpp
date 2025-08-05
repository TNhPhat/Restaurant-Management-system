#pragma once

#include <vector>
#include <memory>
#include "Bill.hpp"
#include "FilterCriteria.hpp"

class IFilterStrategy {
public:
    virtual ~IFilterStrategy() = default;
    virtual std::vector<std::shared_ptr<Bill>> Filter(
        const std::vector<std::shared_ptr<Bill>>& bills,
        const FilterCriteria& criteria
    ) const = 0;
};

class BillFilterStrategy : public IFilterStrategy {
public:
    std::vector<std::shared_ptr<Bill>> Filter(
        const std::vector<std::shared_ptr<Bill>>& bills,
        const FilterCriteria& criteria
    ) const override;

private:
    bool PassesAmountFilter(const std::shared_ptr<Bill>& bill, const AmountRange& range) const;
    bool PassesDateFilter(const std::shared_ptr<Bill>& bill, const DateRange& range) const;
    bool PassesBillTypeFilter(const std::shared_ptr<Bill>& bill, BillType type) const;
};