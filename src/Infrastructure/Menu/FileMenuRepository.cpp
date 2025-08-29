#include "FileMenuRepository.hpp"

FileMenuRepository::FileMenuRepository(std::string MenuFilePath, std::string ItemFilePath, std::string AddonFilePath) {
    this->m_FileHandle = std::make_unique<JsonHandle>();
    this->m_FileHandle->LoadFile(AddonFilePath);
    auto data = this->m_FileHandle->GetDaTa();
    if (data.is_array() == false) {
        LOG_ERROR("FileMenuAddonRepository: Invalid data format in file {}", AddonFilePath);
        return;
    }
    for (const auto &addonData: data) {
        int addonID = addonData["ID"].get<int>();
        std::string name = addonData["Name"].get<std::string>();
        double price = addonData["Price"].get<double>();

        std::vector<MealIngredient> ingredients;
        for (const auto &ingredientData: addonData["Ingredients"]) {
            MealIngredient ingredient(ingredientData["Name"].get<std::string>(),
                                      ingredientData["Quantity"].get<int>());
            ingredients.push_back(ingredient);
        }
        const std::shared_ptr<MenuAddon> MAPtr = std::make_shared<MenuAddon>(addonID, name, price, ingredients);
        this->SaveMenuAddon(MAPtr);
    }
    this->m_FileHandle = std::make_unique<JsonHandle>();
    this->m_FileHandle->LoadFile(ItemFilePath);
    data = this->m_FileHandle->GetDaTa();
    if (data.is_array() == false) {
        LOG_ERROR("FileMenuItemRepository: Invalid data format in file {}", ItemFilePath);
        return;
    }
    for (const auto &itemData: data) {
        int itemID = itemData["MenuItemID"].get<int>();
        auto title = itemData["Title"].get<std::string>();
        auto description = itemData["Description"].get<std::string>();
        double price = itemData["Price"].get<double>();

        std::vector<MealIngredient> ingredients;
        for (const auto &ingredientData: itemData["Ingredients"]) {
            MealIngredient ingredient(ingredientData["Name"].get<std::string>(),
                                      ingredientData["Quantity"].get<int>());
            ingredients.push_back(ingredient);
        }

        std::vector<std::shared_ptr<MenuAddon> > addons;
        for (const auto &addonData: itemData["AvailableAddons"]) {
            int addonID = addonData["ID"].get<int>();
            if (auto menuAddon = GetMenuAddonByID(addonID); menuAddon != nullptr) {
                addons.push_back(menuAddon);
            } else {
                LOG_ERROR("MenuAddon with ID {} not found for MenuItem with ID {}.", addonID, itemID);
            }
        }
        auto menuItemPtr = this->SaveMenuItem(
            std::make_shared<MenuItem>(itemID, title, description, price));
        for (const auto &ingredient: ingredients) {
            menuItemPtr->AddIngredient(ingredient.Name, ingredient.Quantity);
        }
        for (const auto &addon: addons) {
            menuItemPtr->AddAddon(addon);
        }
    }
    this->m_FileHandle = std::make_unique<JsonHandle>();
    this->m_FileHandle->LoadFile(MenuFilePath);
    data = this->m_FileHandle->GetDaTa();
    if (data.is_array() == false) {
        LOG_ERROR("FileMenuRepository: Invalid data format in file {}", MenuFilePath);
        return;
    }
    for (const auto &menuData: data) {
        int menuID = menuData["ID"].get<int>();
        auto name = menuData["Name"].get<std::string>();
        auto description = menuData["Description"].get<std::string>();

        auto menuPtr = this->SaveMenu(std::make_shared<Menu>(menuID, name, description));
        for (const auto &sectionData: menuData["Sections"]) {
            int sectionID = sectionData["ID"].get<int>();
            auto title = sectionData["Title"].get<std::string>();
            auto sectionDescription = sectionData["Description"].get<std::string>();

            const auto sectionPtr = std::make_shared<MenuSection>(sectionID, title, sectionDescription, menuID);
            this->SaveSection(menuPtr, sectionPtr);
            for (const auto &itemData: sectionData["Menu Items"]) {
                int itemID = itemData.get<int>();
                if (const auto menuItem = GetMenuItemByID(itemID); menuItem != nullptr) {
                    sectionPtr->AddMenuItem(menuItem);
                } else {
                    LOG_ERROR("MenuItem with ID {} not found for MenuSection with ID {}.", itemID, sectionID);
                }
            }
        }
    }
}

void FileMenuRepository::SaveMenus(std::string filePath) const {
    json data = json::array();
    for (const auto &menu: this->GetMenus()) {
        json menuJson;
        menuJson["ID"] = menu->GetID();
        menuJson["Name"] = menu->GetName();
        menuJson["Description"] = menu->GetDescription();
        menuJson["Sections"] = json::array();

        for (const auto &section: menu->GetSections()) {
            json sectionJson;
            sectionJson["ID"] = section->GetID();
            sectionJson["Title"] = section->GetTitle();
            sectionJson["Description"] = section->GetDescription();
            sectionJson["Menu Items"] = json::array();

            for (const auto &item: section->GetMenuItems()) {
                sectionJson["Menu Items"].push_back(item->GetID());
            }

            menuJson["Sections"].push_back(sectionJson);
        }

        data.push_back(menuJson);
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }
    outFile << data.dump(4);
    outFile.close();
    LOG_INFO("Menus saved successfully to {}", filePath);
}

void FileMenuRepository::SaveItems(std::string filePath) const {
    json data = json::array();
    for (const auto &item: this->GetMenuItems()) {
        json itemJson;
        itemJson["MenuItemID"] = item->GetID();
        itemJson["Title"] = item->GetTitle();
        itemJson["Description"] = item->GetDescription();
        itemJson["Price"] = item->GetPrice();
        itemJson["Ingredients"] = json::array();

        for (const auto &ingredient: item->GetIngredients()) {
            json ingredientJson;
            ingredientJson["Name"] = ingredient.Name;
            ingredientJson["Quantity"] = ingredient.Quantity;
            itemJson["Ingredients"].push_back(ingredientJson);
        }

        itemJson["AvailableAddons"] = json::array();
        for (const auto &addon: item->GetAvailableAddons()) {
            json addonJson;
            addonJson["ID"] = addon->GetID();
            addonJson["Name"] = addon->GetName();
            addonJson["Price"] = addon->GetPrice();
            itemJson["AvailableAddons"].push_back(addonJson);
        }

        data.push_back(itemJson);
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }
    outFile << data.dump(4);
    outFile.close();
    LOG_INFO("MenuItems saved successfully to {}", filePath);
}

void FileMenuRepository::SaveAddons(std::string filePath) const {
    json data = json::array();
    for (const auto &addon: this->GetMenuAddons()) {
        json addonJson;
        addonJson["ID"] = addon->GetID();
        addonJson["Name"] = addon->GetName();
        addonJson["Price"] = addon->GetPrice();
        addonJson["Ingredients"] = json::array();

        for (const auto &ingredient: addon->GetIngredients()) {
            json ingredientJson;
            ingredientJson["Name"] = ingredient.Name;
            ingredientJson["Quantity"] = ingredient.Quantity;
            addonJson["Ingredients"].push_back(ingredientJson);
        }

        data.push_back(addonJson);
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }
    outFile << data.dump(4);
    outFile.close();
    LOG_INFO("MenuAddons saved successfully to {}", filePath);
}
