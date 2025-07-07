#pragma once 

#include "DateTime.hpp"
#include "OrderStatus.hpp"
#include "Meal.hpp"

class Order {
private:
    const int m_OrderID;
    int m_TableID;
    int m_CustomerID;
    DateTime m_Date;
    OrderStatus m_Status;
    std::vector<std::shared_ptr<Meal>> m_Meals;
public: 
    Order(const int &OrderID, const int &TableID, const DateTime &Date);
    ~Order() = default;
    DateTime GetDate() const; 
    double GetTotalPrice() const; 

    void SetOrderStatus(const OrderStatus &NewOrderStatus);
    std::vector<std::shared_ptr<Meal>> GetMeals();
    bool AddMeal(const std::shared_ptr<Meal>& NewMeal);
    bool RemoveMeal(const int &MealID);
};