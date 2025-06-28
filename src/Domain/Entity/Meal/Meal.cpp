#include "Meal.hpp"
#include "../../../Core/Utils/IDManager.hpp"

MealItem::MealItem(const MenuItem &Item, int Quantity) : m_MenuItem(Item), m_Quantity(Quantity), m_MealItemID(IDManager::GetInstance().GetNextID("MealItem"))
{
}

MealItem::MealItem(const MealItem &Origin) : m_MenuItem(Origin.m_MenuItem), m_Quantity(Origin.m_Quantity), m_MealItemID(Origin.m_MealItemID)
{
}

double MealItem::GetPrice() const
{
    return this->m_Quantity * this->m_MenuItem.GetPrice();
}

int MealItem::GetID() const
{
    return this->m_MealItemID;
}

int MealItem::GetQuantity() const
{
    return this->m_Quantity;
}

void MealItem::SetQuantity(int Quantity)
{
    this->m_Quantity = Quantity;
}

std::vector<MealIngredient> MealItem::GetResources() const
{
    return this->m_MenuItem.GetIngredients();
}

Meal::Meal() : m_MealID(IDManager::GetInstance().GetNextID("Meal"))
{
}

void Meal::AddItem(std::shared_ptr<MealItem> Item)
{
    this->m_MealItems.emplace_back(Item);
}

template <typename... Args>
void Meal::AddItem(Args &&...args)
{
    AddItem(std::make_shared<const MealItem>(std::forward<Args>(args)...));
}

int Meal::GetID() const
{
    return this->m_MealID;
}

double Meal::GetPrice() const
{
    double total = 0;
    for (size_t i = 0; i < m_MealItems.size(); i++)
    {
        total += m_MealItems[i]->GetPrice();
    }
    return total;
}

std::vector<std::shared_ptr<MealItem>> Meal::GetMealItems() const
{
    return this->m_MealItems;
}