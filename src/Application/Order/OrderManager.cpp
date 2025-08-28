#include "OrderManager.hpp"
#include "Logger.hpp"

OrderManager::OrderManager(std::unique_ptr<OrderRepository> &Repository,
                           std::unique_ptr<OrderService> &Service): m_Repository(std::move(Repository)),
                                                                    m_Service(std::move(Service)) {
    m_Orders = m_Repository->LoadAllOrders();
}

OrderManager::~OrderManager() {
    m_Repository->SaveAllOrders(m_Orders);
}

void OrderManager::CreateOrder(const int &tableID, const std::string &customerPhone,
                               const DateTime &date, const std::vector<std::shared_ptr<Meal> > &meals) {
    auto order = m_Service->GenerateOrder(tableID, customerPhone, date, meals);
    m_Orders.push_back(order);
}

void OrderManager::UpdateOrderStatus(const int &orderID, const OrderStatus &newStatus) {
    for (auto &order: m_Orders) {
        if (order->GetID() == orderID) {
            order->SetOrderStatus(newStatus);
            return;
        }
    }
    LOG_WARNING("Order ID {} not found for status update.", orderID);
}

bool OrderManager::RemoveOrder(const int &orderID) {
    auto it = std::remove_if(m_Orders.begin(), m_Orders.end(),
                             [orderID](const std::shared_ptr<Order> &o) { return o->GetID() == orderID; });

    if (it != m_Orders.end()) {
        m_Orders.erase(it, m_Orders.end());
        return true;
    }
    LOG_WARNING("Order ID {} not found for removal.", orderID);
    return false;
}

std::vector<std::shared_ptr<Order> > OrderManager::GetAllOrders() const {
    return m_Orders;
}

void OrderManager::SaveAllOrders() {
    m_Repository->SaveAllOrders(m_Orders);
}

std::vector<std::shared_ptr<Order> > OrderManager::GetOrdersInDateRange(const DateTime &startDate,
                                                                        const DateTime &endDate) const {
    return m_Service->FilterByDateRange(m_Orders, startDate, endDate);
}
