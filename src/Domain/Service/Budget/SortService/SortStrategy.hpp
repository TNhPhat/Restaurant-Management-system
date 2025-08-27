#pragma once

#include <vector>
#include <memory>
#include "Bill.hpp"

enum class SortCriteria {
    ID = 0,
    Date,
    Message,
    Amount,
    BillType
};

class ISortStrategy {
public:
    virtual ~ISortStrategy() = default;

    virtual void Sort(std::vector<std::shared_ptr<Bill> > &bills) const = 0;
};

// Concrete sorting strategies
class SortByDateStrategy : public ISortStrategy {
public:
    void Sort(std::vector<std::shared_ptr<Bill> > &bills) const override;
};

class SortByAmountStrategy : public ISortStrategy {
public:
    void Sort(std::vector<std::shared_ptr<Bill> > &bills) const override;
};

class SortByBillTypeStrategy : public ISortStrategy {
public:
    void Sort(std::vector<std::shared_ptr<Bill> > &bills) const override;
};

class SortByIDStrategy : public ISortStrategy {
public:
    void Sort(std::vector<std::shared_ptr<Bill> > &bills) const override;
};

class SortByMessageStrategy : public ISortStrategy {
public:
    void Sort(std::vector<std::shared_ptr<Bill> > &bills) const override;
};

// Factory for creating sort strategies
class SortStrategyFactory {
public:
    static std::unique_ptr<ISortStrategy> CreateStrategy(SortCriteria criteria);
};
