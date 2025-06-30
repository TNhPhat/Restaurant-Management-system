#include "MealManager.hpp"

#include "Application/Menu/MenuManager.hpp"

MealManager::MealManager() : m_IsLoaded(false)
{
    m_FileHandle = std::make_unique<JsonHandle>();
    m_Meals.reserve(100); // Reserve space for 100 meals to optimize performance
}

MealManager &MealManager::GetInstance()
{
    static MealManager instance;
    return instance;
}

void MealManager::LoadMealsFromFile(const std::string &FilePath)
{
    if (this->IsLoaded() == true)
        return;
    if (MenuItemManager::GetInstance().IsLoaded() == false)
    {
        throw std::runtime_error("Menu items must be loaded before loading meals.");
    }
    if (MenuAddonManager::GetInstance().IsLoaded() == false)
    {
        throw std::runtime_error("Menu addons must be loaded before loading meals.");
    }
    try
    {
        this->m_FileHandle->LoadFile(FilePath);
        json data = this->m_FileHandle->GetDaTa();
        this->m_Meals.clear();

        for (const auto &mealJson : data)
        {
            int mealID = mealJson["MealID"];
            // std::string mealDate = mealJson["MealDate"];

            auto meal = std::make_shared<Meal>(mealID);

            // Process each item in the meal
            for (const auto &itemJson : mealJson["Items"])
            {
                const int mealItemID = itemJson["MealItemID"];
                const int menuItemID = itemJson["MenuItemID"];
                const int quantity = itemJson["Quantity"];

                auto menuItem = MenuItemManager::GetInstance().GetMenuItemByID(menuItemID);

                auto mealItem = std::make_shared<MealItem>(
                    mealItemID, *menuItem, quantity, "");

                // Process addons
                for (const auto &addonJson : itemJson["Addons"])
                {
                    int addonID = addonJson["AddonID"];
                    int addonQuantity = addonJson["Quantity"];
                    try
                    {
                        auto addon = MenuAddonManager::GetInstance().GetMenuAddonByID(addonID);
                        mealItem->AddAddon(addon, addonQuantity);
                    }
                    catch (const std::exception &e)
                    {
                        LOG_ERROR("Failed to load addon with ID {}: {}", addonID, e.what());
                    }
                }

                meal->AddItem(mealItem);
            }

            this->m_Meals.push_back(meal);
        }

        this->m_IsLoaded = true;
        LOG_INFO("Successfully loaded {} meals from {}", this->m_Meals.size(), FilePath);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Error loading meals from file {}: {}", FilePath, e.what());
        this->m_IsLoaded = false;
    }
}

void MealManager::SaveMealsToFile(const std::string &FilePath) const
{
    try
    {
        json data = json::array();

        for (const auto &meal : this->m_Meals)
        {
            json mealJson;
            mealJson["MealID"] = meal->GetID();

            // TODO: Store actual meal date when Meal class supports it
            mealJson["MealDate"] = "2025-06-30T12:00:00Z";

            json itemsArray = json::array();

            for (const auto &mealItem : meal->GetMealItems())
            {
                json itemJson;

                itemJson["MealItemID"] = mealItem->GetID();
                itemJson["MenuItemID"] = mealItem->GetMenuItem().GetID();
                itemJson["Quantity"] = mealItem->GetQuantity();

                json addonsArray = json::array();

                for (const auto &mealAddon : mealItem->GetAddons())
                {
                    json addonJson;
                    addonJson["AddonID"] = mealAddon->GetAddon()->GetID();
                    addonJson["Quantity"] = mealAddon->GetQuantity();
                    addonsArray.push_back(addonJson);
                }

                itemJson["Addons"] = addonsArray;
                itemsArray.push_back(itemJson);
            }

            mealJson["Items"] = itemsArray;
            data.push_back(mealJson);
        }

        std::ofstream file(FilePath);
        if (!file.is_open())
        {
            LOG_ERROR("Cannot open file for writing: {}", FilePath);
        }
        file << data.dump(4);
        file.close();
        LOG_INFO("Successfully saved {} meals to {}", this->m_Meals.size(), FilePath);
    }
    catch (const std::exception &e)
    {
        LOG_ERROR("Error saving meals to file {}: {}", FilePath, e.what());
    }
}

std::vector<std::shared_ptr<Meal>> MealManager::GetMeals() const
{
    return this->m_Meals;
}

std::shared_ptr<Meal> MealManager::GetMealByID(const int ID) const
{
    for (auto &Item : this->m_Meals)
    {
        if (Item->GetID() == ID)
        {
            return Item;
        }
    }
    throw std::invalid_argument("Meal with ID " + std::to_string(ID) + " not found");
}

void MealManager::AddMeal(const std::shared_ptr<Meal> &Meal)
{
    if (Meal == nullptr)
    {
        throw std::invalid_argument("Meal cannot be null");
    }
    this->m_Meals.push_back(Meal);
}

void MealManager::RemoveMeal(const std::shared_ptr<Meal> &Meal)
{
    if (Meal == nullptr)
    {
        throw std::invalid_argument("Meal cannot be null");
    }
    for (const auto &Item : this->m_Meals)
    {
        if (Item->GetID() == Meal->GetID())
        {
            std::erase(this->m_Meals, Item);
            return;
        }
    }
    throw std::invalid_argument("Meal with ID " + std::to_string(Meal->GetID()) + " not found");
}

bool MealManager::IsLoaded() const
{
    return this->m_IsLoaded;
}
