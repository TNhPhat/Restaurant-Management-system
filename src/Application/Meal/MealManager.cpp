#include "MealManager.hpp"

#include "Application/Menu/MenuManager.hpp"

MealManager::MealManager(FileMealRepository &MealRepository): m_MealRepository(MealRepository) {
}

std::vector<std::shared_ptr<Meal> > MealManager::GetMeals() const {
    return this->m_MealRepository.GetMeals();
}

std::shared_ptr<Meal> MealManager::GetMealByID(int MealID) const {
    return this->m_MealRepository.GetMealByID(MealID);
}


std::shared_ptr<Meal> MealManager::SaveMeal(const std::shared_ptr<Meal> &MealPtr) const {
    return this->m_MealRepository.SaveMeal(MealPtr);
}

void MealManager::RemoveMeal(const std::shared_ptr<Meal> &Meal) const {
    return this->m_MealRepository.RemoveMeal(Meal);
}

void MealManager::RemoveMeal(const int MealID) const {
    return this->m_MealRepository.RemoveMeal(MealID);
}

std::vector<std::shared_ptr<MealItem> > MealManager::GetMealItemsFromMeal(const int MealID) const {
    return this->m_MealRepository.GetMealByID(MealID)->GetMealItems();
}

std::shared_ptr<MealItem> MealManager::GetMealItemByID(const int ID) const {
    return this->m_MealRepository.GetMealItemByID(ID);
}

std::shared_ptr<MealItem> MealManager::SaveMealItem(const std::shared_ptr<MealItem> &MealItemPtr) const {
    return this->m_MealRepository.SaveMealItem(MealItemPtr);
}

void MealManager::RemoveMealItem(const std::shared_ptr<MealItem> &MealItem) const {
    return this->m_MealRepository.RemoveMealItem(MealItem);
}

void MealManager::RemoveMealItem(const int MealItemID) const {
    return this->m_MealRepository.RemoveMealItem(MealItemID);
}

std::vector<std::shared_ptr<MealAddon> > MealManager::GetAvailableAddonsFromItem(const int MealItemID) const {
    return this->m_MealRepository.GetMealItemByID(MealItemID)->GetAddons();
}


std::shared_ptr<MealAddon> MealManager::GetMealAddonByID(int MealAddonID) const {
    return this->m_MealRepository.GetMealAddonByID(MealAddonID);
}

std::shared_ptr<MealAddon> MealManager::GetMealAddonByName(const std::string &MealAddonName) const {
    return this->m_MealRepository.GetMealAddonByName(MealAddonName);
}

std::vector<std::shared_ptr<MealAddon> > MealManager::GetMealAddons() const {
    return this->m_MealRepository.GetMealAddons();
}

std::shared_ptr<MealAddon> MealManager::SaveMealAddon(const std::shared_ptr<MealAddon> &MealAddonPtr) const {
    return this->m_MealRepository.SaveMealAddon(MealAddonPtr);
}

void MealManager::RemoveMealAddon(const std::shared_ptr<MealAddon> &MealAddon) const {
    return this->m_MealRepository.RemoveMealAddon(MealAddon);
}

void MealManager::RemoveMealAddon(const int MealAddonID) const {
    return this->m_MealRepository.RemoveMealAddon(MealAddonID);
}
