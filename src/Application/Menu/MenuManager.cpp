#include "MenuManager.hpp"

MenuAddonManager::MenuAddonManager() : m_IsLoaded(false)
{
    m_FileHandle = std::make_unique<JsonHandle>();
}

MenuAddonManager &MenuAddonManager::GetInstance()
{
    static MenuAddonManager instance;
    return instance;
}

void MenuAddonManager::LoadMenuAddonsFromFile(const std::string &FilePath)
{
    if (this->IsLoaded() == true)
        return;
    this->m_FileHandle->LoadFile(FilePath);
    try
    {
        json jsonData = this->m_FileHandle->GetDaTa();

        this->m_MenuAddons.clear();

        if (jsonData.is_array())
        {
            for (const auto &addonJson : jsonData)
            {
                int id = addonJson["ID"];
                std::string name = addonJson["Name"];
                double price = addonJson["Price"];

                std::vector<MealIngredient> ingredients;
                if (addonJson.contains("Ingredients") && addonJson["Ingredients"].is_array())
                {
                    for (const auto &ingredientJson : addonJson["Ingredients"])
                    {
                        MealIngredient ingredient;
                        ingredient.Name = ingredientJson["Name"];
                        ingredient.Quantity = ingredientJson["Quantity"];
                        ingredients.push_back(ingredient);
                    }
                }

                auto menuAddon = std::make_shared<MenuAddon>(id, name, price, ingredients);

                this->m_MenuAddons.push_back(menuAddon);
            }
        }

        this->m_IsLoaded = true;
    }
    catch (const std::exception &e)
    {
        this->m_IsLoaded = false;
        throw std::runtime_error("Failed to load menu addons from file: " + std::string(e.what()));
    }
}

void MenuAddonManager::SaveMenuAddonsToFile(const std::string &FilePath) const
{
    try
    {
        json jsonArray = json::array();

        for (const auto &menuAddon : this->m_MenuAddons)
        {
            json addonJson;

            addonJson["ID"] = menuAddon->GetID();
            addonJson["Name"] = menuAddon->GetName();
            addonJson["Price"] = menuAddon->GetPrice();

            json ingredientsArray = json::array();
            for (const auto &ingredient : menuAddon->GetIngredients())
            {
                json ingredientJson;
                ingredientJson["Name"] = ingredient.Name;
                ingredientJson["Quantity"] = ingredient.Quantity;
                ingredientsArray.push_back(ingredientJson);
            }
            addonJson["Ingredients"] = ingredientsArray;

            jsonArray.push_back(addonJson);
        }

        std::ofstream file(FilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file for writing: " + FilePath);
        }

        file << jsonArray.dump(4);
        file.close();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to save menu addons to file: " + std::string(e.what()));
    }
}

std::vector<std::shared_ptr<MenuAddon>> MenuAddonManager::GetMenuAddons() const
{
    return this->m_MenuAddons;
}

std::shared_ptr<MenuAddon> MenuAddonManager::GetMenuAddonByID(const int ID) const
{
    for (auto &Addon : this->m_MenuAddons)
    {
        if (Addon->GetID() == ID)
        {
            return Addon;
        }
    }
    throw std::invalid_argument("MenuAddon with ID " + std::to_string(ID) + " not found");
}

std::shared_ptr<MenuAddon> MenuAddonManager::GetMenuAddonByName(const std::string &Name) const
{
    for (auto &Addon : this->m_MenuAddons)
    {
        if (Addon->GetName() == Name)
        {
            return Addon;
        }
    }
    throw std::invalid_argument("MenuAddon with name '" + Name + "' not found");
}

void MenuAddonManager::AddMenuAddon(const std::shared_ptr<MenuAddon> &Addon)
{
    if (Addon == nullptr)
    {
        throw std::invalid_argument("MenuAddon cannot be null");
    }
    this->m_MenuAddons.push_back(Addon);
}

void MenuAddonManager::RemoveMenuAddon(const std::shared_ptr<MenuAddon> &Addon)
{
    if (Addon == nullptr)
    {
        throw std::invalid_argument("MenuAddon cannot be null");
    }
    for (const auto &AddonPtr : this->m_MenuAddons)
    {
        if (AddonPtr->GetID() == Addon->GetID())
        {
            std::erase(this->m_MenuAddons, AddonPtr);
            return;
        }
    }
    throw std::invalid_argument("MenuAddon with ID " + std::to_string(Addon->GetID()) + " not found");
}

bool MenuAddonManager::IsLoaded() const
{
    return this->m_IsLoaded;
}

MenuItemManager::MenuItemManager() : m_IsLoaded(false)
{
    m_FileHandle = std::make_unique<JsonHandle>();
    m_MenuItems.reserve(100);
}

MenuItemManager &MenuItemManager::GetInstance()
{
    static MenuItemManager instance;
    return instance;
}

void MenuItemManager::LoadMenuItemsFromFile(const std::string &FilePath)
{
    if (this->IsLoaded() == true)
        return;
    this->m_FileHandle->LoadFile(FilePath);
    try
    {
        json jsonData = this->m_FileHandle->GetDaTa();

        this->m_MenuItems.clear();

        if (jsonData.is_array())
        {
            for (const auto &itemJson : jsonData)
            {
                int id = itemJson["MenuItemID"];
                std::string title = itemJson["Title"];
                std::string description = itemJson["Description"];
                double price = itemJson["Price"];

                auto menuItem = std::make_shared<MenuItem>(id, title, description, price);

                if (itemJson.contains("Ingredients") && itemJson["Ingredients"].is_array())
                {
                    for (const auto &ingredientJson : itemJson["Ingredients"])
                    {
                        const std::string ingredientName = ingredientJson["Name"];
                        const int quantity = ingredientJson["Quantity"];
                        menuItem->AddIngredient(ingredientName, quantity);
                    }
                }

                if (itemJson.contains("AvailableAddons") && itemJson["AvailableAddons"].is_array())
                {
                    for (const auto &AddonsID : itemJson["AvailableAddons"])
                    {
                        menuItem->AddAddon(MenuAddonManager::GetInstance().GetMenuAddonByID(AddonsID));
                    }
                }
                this->m_MenuItems.push_back(menuItem);
            }
        }
        this->m_IsLoaded = true;
    }
    catch (const std::exception &e)
    {
        this->m_IsLoaded = false;
        throw std::runtime_error("Failed to load menu items from file: " + std::string(e.what()));
    }
}

void MenuItemManager::SaveMenuItemsToFile(const std::string &FilePath) const
{
    try
    {
        json jsonArray = json::array();

        for (const auto &menuItem : this->m_MenuItems)
        {
            json itemJson;

            itemJson["MenuItemID"] = menuItem->GetID();
            itemJson["Title"] = menuItem->GetTitle();
            itemJson["Description"] = menuItem->GetDescription();
            itemJson["Price"] = menuItem->GetPrice();

            json ingredientsArray = json::array();
            for (const auto &[Name, Quantity] : menuItem->GetIngredients())
            {
                json ingredientJson;
                ingredientJson["Name"] = Name;
                ingredientJson["Quantity"] = Quantity;
                ingredientsArray.push_back(ingredientJson);
            }
            itemJson["Ingredients"] = ingredientsArray;

            itemJson["AvailableAddons"] = json::array();

            for (const auto &addon : menuItem->GetAvailableAddons())
            {
                itemJson["AvailableAddons"].push_back(addon->GetID());
            }

            jsonArray.push_back(itemJson);
        }

        std::ofstream file(FilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file for writing: " + FilePath);
        }

        file << jsonArray.dump(4);
        file.close();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to save menu items to file: " + std::string(e.what()));
    }
}

std::vector<std::shared_ptr<MenuItem>> MenuItemManager::GetMenuItems() const
{
    return this->m_MenuItems;
}

void MenuItemManager::AddMenuItem(const std::shared_ptr<MenuItem> &Item)
{
    if (Item == nullptr)
    {
        throw std::invalid_argument("MenuItem cannot be null");
    }
    this->m_MenuItems.emplace_back(Item);
}

void MenuItemManager::RemoveMenuItem(const int ItemID)
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetID() == ItemID)
        {
            std::erase(this->m_MenuItems, Item);
            return;
        }
    }
    throw std::invalid_argument("MenuItem with ID " + std::to_string(ItemID) + " not found");
}

std::shared_ptr<MenuItem> MenuItemManager::GetMenuItemByID(const int ItemID) const
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetID() == ItemID)
        {
            return Item;
        }
    }
    throw std::invalid_argument("MenuItem with ID " + std::to_string(ItemID) + " not found");
}

std::shared_ptr<MenuItem> MenuItemManager::GetMenuItemByName(const std::string &Name) const
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetTitle() == Name)
        {
            return Item;
        }
    }
    throw std::invalid_argument("MenuItem with name '" + Name + "' not found");
}

bool MenuItemManager::IsLoaded() const
{
    return this->m_IsLoaded;
}

MenuManager::MenuManager() : m_IsLoaded(false)
{
    m_FileHandle = std::make_unique<JsonHandle>();
}

MenuManager &MenuManager::GetInstance()
{
    static MenuManager instance;
    return instance;
}

void MenuManager::LoadMenuFromFile(const std::string &FilePath)
{
    if (this->IsLoaded() == true)
        return;
    this->m_FileHandle->LoadFile(FilePath);
    try
    {
        json jsonData = this->m_FileHandle->GetDaTa();

        this->m_MenuItems.clear();

        if (jsonData.is_array())
        {
            for (const auto &menuJson : jsonData)
            {
                int menuId = menuJson["ID"];
                std::string name = menuJson["Name"];
                std::string description = menuJson["Description"];

                auto menu = std::make_shared<Menu>(menuId, name, description);

                if (menuJson.contains("Sections") && menuJson["Sections"].is_array())
                {
                    for (const auto &sectionJson : menuJson["Sections"])
                    {
                        int sectionId = sectionJson["ID"];
                        std::string sectionTitle = sectionJson["Title"];
                        std::string sectionDescription = sectionJson["Description"];

                        const auto menuSection = std::make_shared<MenuSection>(
                            sectionId, sectionTitle, sectionDescription);

                        if (sectionJson.contains("Menu Items") && sectionJson["Menu Items"].is_array())
                        {
                            for (const auto &itemIdJson : sectionJson["Menu Items"])
                            {
                                const int itemId = itemIdJson;

                                try
                                {
                                    std::shared_ptr<MenuItem> menuItem = MenuItemManager::GetInstance().GetMenuItemByID(
                                        itemId);
                                    menuSection->AddMenuItem(menuItem);
                                }
                                catch (const std::exception &e)
                                {
                                    LOG_INFO("MenuItem with ID {} not found: {}", itemId, e.what());
                                }
                            }
                        }

                        menu->AddSection(menuSection);
                    }
                }

                this->m_MenuItems.push_back(menu);
            }
        }
        this->m_IsLoaded = true;
    }
    catch (const std::exception &e)
    {
        this->m_IsLoaded = false;
        throw std::runtime_error("Failed to load menus from file: " + std::string(e.what()));
    }
}

void MenuManager::SaveMenuToFile(const std::string &FilePath) const
{
    try
    {
        json jsonArray = json::array();

        for (const auto &menu : this->m_MenuItems)
        {
            json menuJson;

            menuJson["ID"] = menu->GetID();
            menuJson["Name"] = menu->GetName();
            menuJson["Description"] = menu->GetDescription();

            json sectionsArray = json::array();
            for (const auto &section : menu->GetSections())
            {
                json sectionJson;

                sectionJson["ID"] = section->GetID();
                sectionJson["Title"] = section->GetTitle();
                sectionJson["Description"] = section->GetDescription();

                json menuItemsArray = json::array();
                for (const auto &menuItem : section->GetMenuItems())
                {
                    menuItemsArray.push_back(menuItem->GetID());
                }
                sectionJson["Menu Items"] = menuItemsArray;
                sectionsArray.push_back(sectionJson);
            }
            menuJson["Sections"] = sectionsArray;
            jsonArray.push_back(menuJson);
        }

        std::ofstream file(FilePath);
        if (!file.is_open())
        {
            throw std::runtime_error("Cannot open file for writing: " + FilePath);
        }

        file << jsonArray.dump(4);
        file.close();
    }
    catch (const std::exception &e)
    {
        throw std::runtime_error("Failed to save menus to file: " + std::string(e.what()));
    }
}

std::vector<std::shared_ptr<Menu>> MenuManager::GetMenus() const
{
    return this->m_MenuItems;
}

void MenuManager::AddMenu(const std::shared_ptr<Menu> &Menu)
{
    if (Menu == nullptr)
    {
        throw std::invalid_argument("Menu cannot be null");
    }
    this->m_MenuItems.emplace_back(Menu);
}

void MenuManager::RemoveMenu(const int MenuID)
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetID() == MenuID)
        {
            std::erase(this->m_MenuItems, Item);
            return;
        }
    }
    throw std::invalid_argument("Menu with ID " + std::to_string(MenuID) + " not found");
}

std::shared_ptr<Menu> MenuManager::GetMenuByID(const int MenuID) const
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetID() == MenuID)
        {
            return Item;
        }
    }
    throw std::invalid_argument("Menu with ID " + std::to_string(MenuID) + " not found");
}

std::shared_ptr<Menu> MenuManager::GetMenuByName(const std::string &Name) const
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetName() == Name)
        {
            return Item;
        }
    }
    throw std::invalid_argument("Menu with name '" + Name + "' not found");
}

bool MenuManager::IsLoaded() const
{
    return this->m_IsLoaded;
}
