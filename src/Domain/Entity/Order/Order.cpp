#include <algorithm>
#include "Order.hpp"

Order::Order(const int &OrderID, const int &TableID, const std::string &CustomerPhone, const DateTime &Date)
    : m_Phone(CustomerPhone), m_OrderID(OrderID), m_Date(Date), m_TableID(TableID), m_Status(OrderStatus::None) {
}

Order::~Order() {
    m_Meals.reset();
}

int Order::GetTableID() const {
    return m_TableID;
}

int Order::GetID() const {
    return m_OrderID;
}

std::string Order::GetCustomerPhone() const {
    return m_Phone;
}

OrderStatus Order::GetStatus() const {
    return m_Status;
}

DateTime Order::GetDate() const {
    return m_Date;
}

double Order::GetTotalPrice() const {
    return m_Meals->GetPrice();
}

void Order::SetOrderStatus(const OrderStatus &NewOrderStatus) {
    m_Status = NewOrderStatus;
}

void Order::SetTableID(const int &newID) {
    m_TableID = newID;
}

void Order::SetDate(const DateTime &newDate) {
    m_Date = newDate;
}

void Order::SetCustomerPhone(const std::string &newPhone) {
    m_Phone = newPhone;
}

void Order::SetMeal(const std::shared_ptr<Meal> &meal) {
    m_Meals.reset();
    m_Meals = std::move(meal);
}

std::shared_ptr<Meal> Order::GetMeal() {
    return m_Meals;
}