#include "FileMealRepository.hpp"

FileMealRepository::FileMealRepository(const std::string &filePath,
                                       const IMenuRepository &menuRepository) {
    this->m_FileHandler = std::make_unique<JsonHandle>();
    this->m_FileHandler->LoadFile(filePath);

    auto data = this->m_FileHandler->GetDaTa();
    if (!data.is_array()) {
        LOG_ERROR("FileMealRepository: Invalid data format in file {}", filePath);
        return;
    }
    for (const auto &mealData: data) {
        int MealID = mealData["MealID"].get<int>();
        DateTime mealDate = DateTime::FromDateTimeString(mealData["MealDate"].get<std::string>());

        const auto meal = std::make_shared<Meal>(MealID, mealDate);
        for (const auto &itemData: mealData["Items"]) {
            const int mealItemID = itemData["MealItemID"];
            const int menuItemID = itemData["MenuItemID"];
            const int quantity = itemData["Quantity"];
            const std::string &note = itemData["Notes"];

            auto menuItem = menuRepository.GetMenuItemByID(menuItemID);
            if (menuItem == nullptr) {
                LOG_ERROR("MenuItem with ID {} not found for MealItem with ID {}.", menuItemID, mealItemID);
                continue;
            }

            const auto mealItem = std::make_shared<MealItem>(mealItemID, menuItem, quantity, note);

            for (const auto &addonData: itemData["Addons"]) {
                int addonID = addonData["AddonID"];
                unsigned int addonQuantity = addonData["Quantity"];

                auto menuAddon = menuRepository.GetMenuAddonByID(addonID);
                if (menuAddon == nullptr) {
                    LOG_ERROR("MenuAddon with ID {} not found for MealItem with ID {}.", addonID, mealItemID);
                    continue;
                }

                mealItem->AddAddon(menuAddon, addonQuantity);
            }

            meal->AddItem(mealItem);
        }
        this->SaveMeal(meal);
    }
}

void FileMealRepository::SaveMeals(std::string filePath) const {
    json data = json::array();

    for (const auto &meal: this->GetMeals()) {
        json mealJson;
        mealJson["MealID"] = meal->GetID();
        // mealJson["MealDate"] = meal->GetMealDate();
        mealJson["MealDate"] = "2025-06-30T12:00:00Z";
        mealJson["Items"] = json::array();

        for (const auto &item: meal->GetMealItems()) {
            json itemJson;
            itemJson["MealItemID"] = item->GetID();
            itemJson["MenuItemID"] = item->GetMenuItemID();
            itemJson["Quantity"] = item->GetQuantity();
            itemJson["Note"] = item->GetNote();
            itemJson["Addons"] = json::array();

            for (const auto &addon: item->GetAddons()) {
                json addonJson;
                addonJson["AddonID"] = addon->GetAddon()->GetID();
                addonJson["Quantity"] = addon->GetQuantity();
                itemJson["Addons"].push_back(addonJson);
            }

            mealJson["Items"].push_back(itemJson);
        }

        data.push_back(mealJson);
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }
    outFile << data.dump(4);
    outFile.close();
    LOG_INFO("Meals saved successfully to {}", filePath);
}
