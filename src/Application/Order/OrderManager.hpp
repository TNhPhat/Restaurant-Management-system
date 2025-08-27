#pragma once

#include "Order.hpp"
#include "../../Infrastructure/Order/OrderRepository.hpp"
#include "../../Domain/Service/Order/OrderService.hpp"
#include <memory>

class OrderManager {
private:
    std::shared_ptr<std::vector<std::shared_ptr<Order>>> m_Orders;
    OrderRepository& m_Repository;   // reference instead of unique_ptr
    OrderService& m_Service;         // reference instead of unique_ptr

public:
    OrderManager(std::shared_ptr<std::vector<std::shared_ptr<Order>>>& Orders,
                 OrderRepository& Repository,
                 OrderService& Service);
    ~OrderManager();

    void CreateOrder(const int &tableID, const std::string &customerPhone,
                     const DateTime& date, const std::vector<std::shared_ptr<Meal>>& meals);
    void UpdateOrderStatus(const int &orderID, const OrderStatus &newStatus);
    bool RemoveOrder(const int &orderID);
    std::vector<std::shared_ptr<Order>> GetAllOrders() const;
};
