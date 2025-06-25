#pragma once
#include "Menu.hpp"
#include <vector>
#include <memory>
class MealItem
{
private:
    static int m_MealItemCount;
    const int m_MealItemID;
    int m_Quantity;
    const MenuItem &m_MenuItem;

public:
    MealItem(const MenuItem &Item, int Quantity);
    MealItem(const MealItem &Origin);
    ~MealItem() = default;
    int GetID() const;
    double GetPrice() const;
    int GetQuantity() const;
    void SetQuantity(int Quantity);
    // std::vector<Resource> getResources();
};

class Meal
{
private:
    const int m_MealID;
    static int m_MealCount;
    std::vector<std::shared_ptr<const MealItem>> m_MealItems;

public:
    Meal();
    ~Meal() = default;
    void AddItem(std::shared_ptr<const MealItem> Item);
    template <typename... Args>
    void AddItem(Args &&...args);
    int GetID() const;
    double GetPrice() const;
    const std::vector<std::shared_ptr<const MealItem>> GetMealItems() const;
};