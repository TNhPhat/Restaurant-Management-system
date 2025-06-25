#pragma once
#include "../Entity/Meal/Meal.hpp"

class IMealItemDecorator : public MealItem
{
    static int m_MealItemDecCount;
    const int m_MealItemDecID;
    const MealItem &m_MealItem;
    double m_Price;

public:
    IMealItemDecorator(const MealItem &Item, double Price);
    double GetPrice() const;
    void SetPrice(double Price);
    // std::vector<Resource> getResources();
};
