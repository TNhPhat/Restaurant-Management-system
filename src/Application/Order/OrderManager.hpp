#pragma once

#include "Order.hpp"
#include "OrderRepository.hpp"
#include "OrderService.hpp"
#include <memory>

class OrderManager {
private:
    std::shared_ptr<std::vector<std::shared_ptr<Order>>> m_Orders;
    std::unique_ptr<OrderRepository> m_Repository;
    std::unique_ptr<OrderService> m_Service;

    
public:
    OrderManager(std::shared_ptr<std::vector<std::shared_ptr<Order>>>& Orders,
                 std::unique_ptr<OrderRepository>& Repository,
                 std::unique_ptr<OrderService>& Service);
    ~OrderManager();

    void CreateOrder(const int &tableID, const std::string &customerPhone, const DateTime& date, const std::vector<std::shared_ptr<Meal>>& meals);
    void UpdateOrderStatus(const int &orderID, const OrderStatus &newStatus);
    bool RemoveOrder(const int &orderID);
    std::vector<std::shared_ptr<Order>> GetAllOrders() const;
};
