#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "../Domain/Entity/Order/Order.hpp"
#include "../Core/Utils/IDManager.hpp"

class OrderService {
public:
    std::shared_ptr<Order> GenerateOrder(const int &tableID, const std::string &customerPhone, const DateTime &date,
                                         const std::shared_ptr<Meal> &meals);

    std::vector<std::shared_ptr<Order> > FilterByDate(const std::vector<std::shared_ptr<Order> > &orders,
                                                      const DateTime &date);

    std::vector<std::shared_ptr<Order> > FilterByDateRange(const std::vector<std::shared_ptr<Order> > &orders,
                                                           const DateTime &startDate, const DateTime &endDate);

    std::vector<std::shared_ptr<Order> > FilterByStatus(const std::vector<std::shared_ptr<Order> > &orders,
                                                        OrderStatus status);

    std::vector<std::shared_ptr<Order> > SortByDate(const std::vector<std::shared_ptr<Order> > &orders,
                                                    bool ascending = true);

    std::vector<std::shared_ptr<Order> > SortByTotal(const std::vector<std::shared_ptr<Order> > &orders,
                                                     bool ascending = true);
};
