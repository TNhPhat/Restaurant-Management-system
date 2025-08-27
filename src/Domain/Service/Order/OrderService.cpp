#include "OrderService.hpp"

std::shared_ptr<Order> OrderService::GenerateOrder(const int &tableID, const std::string &customerPhone,
                                                   const DateTime &date,
                                                   const std::vector<std::shared_ptr<Meal> > &meals) {
    int newOrderID = IDManager::GetInstance().GetNextID("Order");
    auto order = std::make_shared<Order>(newOrderID, tableID, customerPhone, date);

    for (const auto &meal: meals) {
        order->AddMeal(meal);
    }

    return order;
}

std::vector<std::shared_ptr<Order> > OrderService::FilterByDate(const std::vector<std::shared_ptr<Order> > &orders,
                                                                const DateTime &date) {
    std::vector<std::shared_ptr<Order> > result;
    std::copy_if(orders.begin(), orders.end(), std::back_inserter(result),
                 [&date](const std::shared_ptr<Order> &o) {
                     return o->GetDate().ToStringDate() == date.ToStringDate();
                 });
    return result;
}

std::vector<std::shared_ptr<Order> > OrderService::FilterByDateRange(const std::vector<std::shared_ptr<Order> > &orders,
                                                                     const DateTime &startDate,
                                                                     const DateTime &endDate) {
    std::vector<std::shared_ptr<Order> > result;
    std::copy_if(orders.begin(), orders.end(), std::back_inserter(result),
                 [&startDate, &endDate](const std::shared_ptr<Order> &o) {
                     return o->GetDate() >= startDate && o->GetDate() <= endDate;
                 });
    return result;
}

std::vector<std::shared_ptr<Order> > OrderService::FilterByStatus(const std::vector<std::shared_ptr<Order> > &orders,
                                                                  OrderStatus status) {
    std::vector<std::shared_ptr<Order> > result;
    std::copy_if(orders.begin(), orders.end(), std::back_inserter(result),
                 [status](const std::shared_ptr<Order> &o) {
                     return o->GetStatus() == status;
                 });
    return result;
}

std::vector<std::shared_ptr<Order> > OrderService::SortByDate(const std::vector<std::shared_ptr<Order> > &orders,
                                                              bool ascending) {
    auto sorted = orders;
    std::sort(sorted.begin(), sorted.end(),
              [ascending](const std::shared_ptr<Order> &a, const std::shared_ptr<Order> &b) {
                  return ascending ? (a->GetDate() < b->GetDate()) : (a->GetDate() > b->GetDate());
              });
    return sorted;
}

std::vector<std::shared_ptr<Order> > OrderService::SortByTotal(const std::vector<std::shared_ptr<Order> > &orders,
                                                               bool ascending) {
    auto sorted = orders;
    std::sort(sorted.begin(), sorted.end(),
              [ascending](const std::shared_ptr<Order> &a, const std::shared_ptr<Order> &b) {
                  return ascending
                             ? (a->GetTotalPrice() < b->GetTotalPrice())
                             : (a->GetTotalPrice() > b->GetTotalPrice());
              });
    return sorted;
}
