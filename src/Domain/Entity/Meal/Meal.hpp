#pragma once
#include "../Menu/Menu.hpp"
#include "../Storage/Resource.hpp"
#include <vector>
#include <memory>

class MealItem
{
private:
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
    virtual std::vector<MealIngredient> GetResources() const;
    void SetQuantity(int Quantity);
    void AddIngredient(std::string Ingredient, int Quantity);
    void RemoveIngredient(std::string Ingredient);
};

class Meal
{
private:
    const int m_MealID;
    std::vector<std::shared_ptr<MealItem>> m_MealItems;

public:
    Meal();
    ~Meal() = default;
    void AddItem(std::shared_ptr<MealItem> Item);
    template <typename... Args>
    void AddItem(Args &&...args);
    int GetID() const;
    double GetPrice() const;
    std::vector<std::shared_ptr<MealItem>> GetMealItems() const;
};