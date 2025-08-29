#pragma once

#include "DateTime.hpp"
#include "OrderStatus.hpp"
#include "Meal.hpp"

class Order {
private:
    const int m_OrderID;
    int m_TableID;
    std::string m_Phone;
    DateTime m_Date;
    OrderStatus m_Status;
    std::shared_ptr<Meal> m_Meals;

public:
    Order(const int &OrderID, const int &TableID, const std::string &CustomerPhone, const DateTime &Date);

    ~Order();

    int GetID() const;

    int GetTableID() const;

    std::string GetCustomerPhone() const;

    OrderStatus GetStatus() const;

    DateTime GetDate() const;

    double GetTotalPrice() const;

    void SetOrderStatus(const OrderStatus &NewOrderStatus);

    void SetTableID(const int &newID);

    void SetDate(const DateTime &newDate);

    void SetCustomerPhone(const std::string &newPhone);

    void SetMeal(const std::shared_ptr<Meal> &meal);

    std::shared_ptr<Meal> GetMeal();
};
