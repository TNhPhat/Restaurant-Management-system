#pragma once 

#include "DateTime.hpp"
#include "OrderStatus.hpp"
#include "Meal.hpp"

class Order {
private:
    const int m_OrderID;
    DateTime m_Date;
    OrderStatus m_Status;
    std::vector<const Meal&> m_Meals;

public: 
    Order(const int &OrderID, const DateTime &Date);
    ~Order() = default;
    DateTime GetDate() const; 
    double GetTotalPrice() const; 

    void SetOrderStatus(const OrderStatus &NewOrderStatus);

    bool AddMeal(const Meal &NewMeal);
    bool RemoveMeal(const int &MealID);
};