#include "IMeal.hpp"

IMealItemDecorator::IMealItemDecorator(const MealItem &Item, double Price) : m_MealItem(Item), MealItem(Item), m_Price(Price), m_MealItemDecID(m_MealItemDecCount++)
{
}

double IMealItemDecorator::GetPrice() const
{
    return m_Price + m_MealItem.GetPrice();
}

void IMealItemDecorator::SetPrice(double Price)
{
    this->m_Price = Price;
}