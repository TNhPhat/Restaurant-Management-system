#pragma once
#include "Meal/Meal.hpp"

class MealManager {
private:
    MealManager();

    ~MealManager() = default;

    std::vector<std::shared_ptr<Meal> > m_Meals;
    std::unique_ptr<JsonHandle> m_FileHandle;
    bool m_IsLoaded;

public:
    MealManager(const MealManager &) = delete;

    MealManager &operator=(const MealManager &) = delete;

    static MealManager &GetInstance();

    void LoadMealsFromFile(const std::string &FilePath);

    void SaveMealsToFile(const std::string &FilePath) const;

    std::vector<std::shared_ptr<Meal> > GetMeals() const;

    std::shared_ptr<Meal> GetMealByID(int ID) const;

    void AddMeal(const std::shared_ptr<Meal> &Meal);

    void RemoveMeal(const std::shared_ptr<Meal> &Meal);

    bool IsLoaded() const;
};
