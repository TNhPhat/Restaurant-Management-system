#include "SortStrategy.hpp"
#include <algorithm>

void SortByDateStrategy::Sort(std::vector<std::shared_ptr<Bill> > &bills) const {
    std::sort(bills.begin(), bills.end(),
              [](const std::shared_ptr<Bill> &a, const std::shared_ptr<Bill> &b) {
                  return a->GetDate() < b->GetDate(); // Ascending order
              });
}

void SortByAmountStrategy::Sort(std::vector<std::shared_ptr<Bill> > &bills) const {
    std::sort(bills.begin(), bills.end(),
              [](const std::shared_ptr<Bill> &a, const std::shared_ptr<Bill> &b) {
                  return a->GetTotal() < b->GetTotal(); // Ascending order
              });
}

void SortByBillTypeStrategy::Sort(std::vector<std::shared_ptr<Bill> > &bills) const {
    std::sort(bills.begin(), bills.end(),
              [](const std::shared_ptr<Bill> &a, const std::shared_ptr<Bill> &b) {
                  return static_cast<int>(a->GetType()) < static_cast<int>(b->GetType());
              });
}

void SortByIDStrategy::Sort(std::vector<std::shared_ptr<Bill> > &bills) const {
    std::sort(bills.begin(), bills.end(),
              [](const std::shared_ptr<Bill> &a, const std::shared_ptr<Bill> &b) {
                  return a->GetID() < b->GetID(); // Ascending order
              });
}

void SortByMessageStrategy::Sort(std::vector<std::shared_ptr<Bill> > &bills) const {
    std::sort(bills.begin(), bills.end(),
              [](const std::shared_ptr<Bill> &a, const std::shared_ptr<Bill> &b) {
                  return a->GetMessage() < b->GetMessage(); // Ascending order
              });
}

std::unique_ptr<ISortStrategy> SortStrategyFactory::CreateStrategy(SortCriteria criteria) {
    switch (criteria) {
        case SortCriteria::Date:
            return std::make_unique<SortByDateStrategy>();
        case SortCriteria::Amount:
            return std::make_unique<SortByAmountStrategy>();
        case SortCriteria::BillType:
            return std::make_unique<SortByBillTypeStrategy>();
        case SortCriteria::ID:
            return std::make_unique<SortByIDStrategy>();
        case SortCriteria::Message:
            return std::make_unique<SortByMessageStrategy>();
        default:
            return std::make_unique<SortByDateStrategy>(); // Default fallback
    }
}
