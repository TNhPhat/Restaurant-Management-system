#include <algorithm>
#include "Order.hpp"

Order::Order(const int& OrderID, const int &TableID, const int &CustomerID, const DateTime& Date)
    : m_CustomerID(CustomerID), m_OrderID(OrderID), m_Date(Date), m_Status(OrderStatus::None) {}

int Order::GetTableID() const {
    return m_TableID;
}

int Order::GetCustomerID() const {
    return m_CustomerID;
}

OrderStatus Order::GetStatus() const {
    return m_Status;
}

DateTime Order::GetDate() const {
    return m_Date;
}

double Order::GetTotalPrice() const {
    double total = 0.0;
    for (const auto& meal : m_Meals) {
        total += meal->GetPrice();  
    }
    return total;
}

void Order::SetOrderStatus(const OrderStatus& NewOrderStatus) {
    m_Status = NewOrderStatus;
}

std::vector<std::shared_ptr<Meal>> Order::GetMeals(){
    return m_Meals;
}

bool Order::AddMeal(const std::shared_ptr<Meal>& NewMeal) {
    auto it = std::find_if(m_Meals.begin(), m_Meals.end(),
        [&NewMeal](const std::shared_ptr<Meal>& m) {
            return m->GetID() == NewMeal->GetID();
        });
    if (it != m_Meals.end()) {
        return false;  
    }

    m_Meals.push_back(NewMeal);
    return true;
}

bool Order::RemoveMeal(const int &MealID) {
    auto it = std::remove_if(m_Meals.begin(), m_Meals.end(),
        [MealID](const std::shared_ptr<Meal>& m) {
            return m->GetID() == MealID;
        });

    if (it != m_Meals.end()) {
        m_Meals.erase(it, m_Meals.end());
        return true;
    }
    return false;
}
