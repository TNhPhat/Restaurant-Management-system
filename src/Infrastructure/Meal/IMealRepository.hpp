#pragma once
#include <memory>
#include <vector>

#include "FileHandle.hpp"
#include "Meal/Meal.hpp"

class IMealRepository {
public:
    virtual ~IMealRepository() = default;

    virtual void SaveMeals(std::string filePath) const = 0;

    std::shared_ptr<MealAddon> SaveMealAddon(const std::shared_ptr<MealAddon> &addon);

    void RemoveMealAddon(const std::shared_ptr<MealAddon> &addon);

    void RemoveMealAddon(int AddonID);

    std::vector<std::shared_ptr<MealAddon> > GetMealAddons() const;

    std::shared_ptr<MealAddon> GetMealAddonByID(int ID) const;

    std::shared_ptr<MealAddon> GetMealAddonByName(const std::string &Name) const;

    std::shared_ptr<MealItem> SaveMealItem(const std::shared_ptr<MealItem> &item);

    void RemoveMealItem(const std::shared_ptr<MealItem> &item);

    void RemoveMealItem(int ItemID);

    std::vector<std::shared_ptr<MealItem> > GetMealItems() const;

    std::shared_ptr<MealItem> GetMealItemByID(int ID) const;

    std::vector<std::shared_ptr<MealItem> > GetMealItemsByMealID(int MealID) const;

    std::shared_ptr<Meal> SaveMeal(const std::shared_ptr<Meal> &meal);

    void RemoveMeal(const std::shared_ptr<Meal> &meal);

    void RemoveMeal(int MealID);

    std::vector<std::shared_ptr<Meal> > GetMeals() const;

    std::shared_ptr<Meal> GetMealByID(int ID) const;

private:
    std::vector<std::shared_ptr<Meal> > m_Meals;
    std::vector<std::shared_ptr<MealAddon> > m_MealAddons;
    std::vector<std::shared_ptr<MealItem> > m_MealItems;
};
