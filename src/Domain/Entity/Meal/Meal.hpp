#pragma once
#include <vector>
#include <memory>

#include "DateTime.hpp"
#include "MealAddon.hpp"

class MealItem {
private:
    const int m_MealItemID;
    MenuItem &m_MenuItem;
    int m_Quantity;
    std::vector<std::shared_ptr<MealAddon> > m_Addons;
    std::string m_Note;

public:
    MealItem(MenuItem &Item, int Quantity);

    MealItem(MenuItem &Item, int Quantity, const std::string &Note);

    MealItem(int ID, MenuItem &Item, int Quantity, const std::string &Note);

    MealItem(const MealItem &Origin);

    ~MealItem() = default;

    int GetID() const;

    double GetPrice() const;

    int GetQuantity() const;

    std::vector<std::shared_ptr<MealAddon> > GetAddons() const;

    std::vector<MealIngredient> GetResources() const;

    std::string GetNote() const;

    const MenuItem &GetMenuItem() const;

    bool ContainsAddon(const int AddonID) const;

    void SetQuantity(int Quantity);

    void SetNote(const std::string &Note);

    void AddAddon(const std::shared_ptr<MenuAddon> &Addon, unsigned int Quantity = 1);

    void RemoveAddon(const std::string &AddonName, unsigned int Quantity = 1);

    void RemoveAddon(int AddonID, unsigned int Quantity = 1);
};

class Meal {
private:
    const int m_MealID;
    DateTime m_MealDate;
    std::vector<std::shared_ptr<MealItem> > m_MealItems;

public:
    Meal(DateTime date);

    Meal(int ID, DateTime date);

    ~Meal() = default;

    void AddItem(const std::shared_ptr<MealItem> &Item);

    template<typename... Args>
    void AddItem(Args &&... args);

    int GetID() const;

    double GetPrice() const;

    std::vector<std::shared_ptr<MealItem> > GetMealItems() const;

    std::vector<MealIngredient> GetResources() const;

    bool ContainsItem(const int ItemID) const;

    void RemoveItem(const int ItemID);

    void UpdateMealDate(const DateTime &date);

    DateTime GetDate() const;
};

template<typename... Args>
void Meal::AddItem(Args &&... args) {
    AddItem(std::make_shared<MealItem>(std::forward<Args>(args)...));
}
