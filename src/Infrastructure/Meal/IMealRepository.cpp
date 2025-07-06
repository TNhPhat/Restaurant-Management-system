#include "IMealRepository.hpp"

std::shared_ptr<MealAddon> IMealRepository::SaveMealAddon(const std::shared_ptr<MealAddon> &addon)
{
    if (addon == nullptr)
    {
        LOG_ERROR("Error saving MealAddon: Addon is null.");
        return nullptr;
    }
    const auto existingAddon = this->GetMealAddonByID(addon->GetAddon()->GetID());
    if (existingAddon == nullptr)
    {
        this->m_MealAddons.push_back(addon);
        LOG_INFO("MealAddon with ID {} saved successfully.", addon->GetAddon()->GetID());
        return addon;
    }
    existingAddon->SetQuantity(addon->GetQuantity());
    LOG_INFO("MealAddon with ID {} updated successfully.", addon->GetAddon()->GetID());
    return existingAddon;
}

void IMealRepository::RemoveMealAddon(const std::shared_ptr<MealAddon> &addon)
{
    if (addon == nullptr)
    {
        LOG_ERROR("Error removing MealAddon: Addon is null.");
        return;
    }
    this->RemoveMealAddon(addon->GetAddon()->GetID());
    LOG_INFO("MealAddon with ID {} removed successfully.", addon->GetAddon()->GetID());
}

void IMealRepository::RemoveMealAddon(int AddonID)
{
    for (const auto &addon : this->m_MealAddons)
    {
        if (addon->GetAddon()->GetID() == AddonID)
        {
            LOG_INFO("Removing MealAddon with ID {}.", AddonID);
            std::erase(this->m_MealAddons, addon);
            return;
        }
    }
    LOG_ERROR("MealAddon with ID {} not found.", AddonID);
}

std::vector<std::shared_ptr<MealAddon>> IMealRepository::GetMealAddons() const
{
    return this->m_MealAddons;
}

std::shared_ptr<MealAddon> IMealRepository::GetMealAddonByID(int ID) const
{
    for (const auto &addon : this->m_MealAddons)
    {
        if (addon->GetAddon()->GetID() == ID)
        {
            return addon;
        }
    }
    return nullptr;
}

std::shared_ptr<MealAddon> IMealRepository::GetMealAddonByName(const std::string &Name) const
{
    for (const auto &addon : this->m_MealAddons)
    {
        if (addon->GetAddon()->GetName() == Name)
        {
            return addon;
        }
    }
    return nullptr;
}

std::shared_ptr<MealItem> IMealRepository::SaveMealItem(const std::shared_ptr<MealItem> &item)
{
    if (item == nullptr)
    {
        LOG_ERROR("Error saving MealItem: Item is null.");
        return nullptr;
    }
    const auto existingItem = this->GetMealItemByID(item->GetID());
    if (existingItem == nullptr)
    {
        this->m_MealItems.push_back(item);
        LOG_INFO("MealItem with ID {} saved successfully.", item->GetID());
        return item;
    }
    existingItem->SetNote(item->GetNote());
    existingItem->SetQuantity(item->GetQuantity());
    for (const auto &ingredient : existingItem->GetAddons())
    {
        if (item->ContainsAddon(ingredient->GetAddon()->GetID()) == false)
        {
            existingItem->RemoveAddon(ingredient->GetAddon()->GetID(), ingredient->GetQuantity());
        }
    }
    for (const auto &addon : item->GetAddons())
    {
        if (existingItem->ContainsAddon(addon->GetAddon()->GetID()) == false)
        {
            existingItem->AddAddon(addon->GetAddon(), addon->GetQuantity());
        }
    }
    return existingItem;
}

void IMealRepository::RemoveMealItem(const std::shared_ptr<MealItem> &item)
{
    if (item == nullptr)
    {
        LOG_ERROR("Error removing MealItem: Item is null.");
        return;
    }
    this->RemoveMealItem(item->GetID());
    LOG_INFO("MealItem with ID {} removed successfully.", item->GetID());
}

void IMealRepository::RemoveMealItem(int ItemID)
{
    for (const auto &item : this->m_MealItems)
    {
        if (item->GetID() == ItemID)
        {
            LOG_INFO("Removing MealItem with ID {}.", ItemID);
            std::erase(this->m_MealItems, item);
            return;
        }
    }
    LOG_ERROR("MealItem with ID {} not found.", ItemID);
}

std::vector<std::shared_ptr<MealItem>> IMealRepository::GetMealItems() const
{
    return this->m_MealItems;
}

std::shared_ptr<MealItem> IMealRepository::GetMealItemByID(const int ID) const
{
    for (const auto &item : this->m_MealItems)
    {
        if (item->GetID() == ID)
        {
            return item;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<MealItem>> IMealRepository::GetMealItemsByMealID(const int MealID) const
{
    return this->GetMealByID(MealID)->GetMealItems();
}

std::shared_ptr<Meal> IMealRepository::SaveMeal(const std::shared_ptr<Meal> &meal)
{
    if (meal == nullptr)
    {
        LOG_ERROR("Error saving Meal: Meal is null.");
        return nullptr;
    }
    const auto existingMeal = this->GetMealByID(meal->GetID());
    if (existingMeal == nullptr)
    {
        this->m_Meals.push_back(meal);
        LOG_INFO("Meal with ID {} saved successfully.", meal->GetID());
        return meal;
    }
    for (const auto &item : existingMeal->GetMealItems())
    {
        if (meal->ContainsItem(item->GetID()) == false)
        {
            existingMeal->RemoveItem(item->GetID());
        }
    }
    for (const auto &item : meal->GetMealItems())
    {
        if (existingMeal->ContainsItem(item->GetID()) == false)
        {
            existingMeal->AddItem(item);
        }
    }
    LOG_INFO("Meal with ID {} updated successfully.", meal->GetID());
    return existingMeal;
}

void IMealRepository::RemoveMeal(const std::shared_ptr<Meal> &meal)
{
    if (meal == nullptr)
    {
        LOG_ERROR("Error removing Meal: Meal is null.");
        return;
    }
    this->RemoveMeal(meal->GetID());
    LOG_INFO("Meal with ID {} removed successfully.", meal->GetID());
}

void IMealRepository::RemoveMeal(int MealID)
{
    for (const auto &meal : this->m_Meals)
    {
        if (meal->GetID() == MealID)
        {
            LOG_INFO("Removing Meal with ID {}.", MealID);
            std::erase(this->m_Meals, meal);
            return;
        }
    }
    LOG_ERROR("Meal with ID {} not found.", MealID);
}

std::vector<std::shared_ptr<Meal>> IMealRepository::GetMeals() const
{
    return this->m_Meals;
}

std::shared_ptr<Meal> IMealRepository::GetMealByID(int ID) const
{
    for (const auto &meal : this->m_Meals)
    {
        if (meal->GetID() == ID)
        {
            return meal;
        }
    }
    return nullptr;
}
