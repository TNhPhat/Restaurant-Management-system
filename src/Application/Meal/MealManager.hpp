#pragma once
#include "../../Infrastructure/Meal/FileMealRepository.hpp"

class MealManager {
private:
    FileMealRepository &m_MealRepository;

public:
    MealManager(FileMealRepository &MealRepository);

    std::vector<std::shared_ptr<Meal> > GetMeals() const;

    std::shared_ptr<Meal> GetMealByID(int MealID) const;

    std::shared_ptr<Meal> SaveMeal(const std::shared_ptr<Meal> &MealPtr) const;

    void RemoveMeal(const std::shared_ptr<Meal> &Meal) const;

    void RemoveMeal(int MealID) const;

    std::vector<std::shared_ptr<MealItem> > GetMealItemsFromMeal(const int MealID) const;

    std::shared_ptr<MealItem> GetMealItemByID(const int ID) const;

    std::shared_ptr<MealItem> SaveMealItem(const std::shared_ptr<MealItem> &MealItemPtr) const;

    void RemoveMealItem(const std::shared_ptr<MealItem> &MealItem) const;

    void RemoveMealItem(int MealItemID) const;

    std::vector<std::shared_ptr<MealAddon> > GetAvailableAddonsFromItem(const int MealItemID) const;

    std::shared_ptr<MealAddon> GetMealAddonByID(int MealAddonID) const;

    std::shared_ptr<MealAddon> GetMealAddonByName(const std::string &MealAddonName) const;

    std::vector<std::shared_ptr<MealAddon> > GetMealAddons() const;

    std::shared_ptr<MealAddon> SaveMealAddon(const std::shared_ptr<MealAddon> &MealAddonPtr) const;

    void RemoveMealAddon(const std::shared_ptr<MealAddon> &MealAddon) const;

    void RemoveMealAddon(int MealAddonID) const;


    ~MealManager() = default;
};
