#pragma once
#include "Order.hpp"
#include "Storage.hpp"

class Kitchen
{
private:
    const int m_KitchenID;
    Storage &m_Storage;
    std::vector<std::shared_ptr<Order>> m_Orders;
public:
    Kitchen(int n_KitchenID, Storage &n_Storage);
    ~Kitchen() = default;
    bool AddOrder(Order n_Order);
};