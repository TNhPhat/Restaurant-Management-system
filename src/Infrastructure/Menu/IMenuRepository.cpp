#include "IMenuRepository.hpp"

std::shared_ptr<MenuAddon> IMenuAddonRepository::SaveMenuAddon(const std::shared_ptr<MenuAddon> &addon)
{
    if (addon == nullptr)
    {
        LOG_ERROR("Error saving MenuAddon: Addon is null.");
        return nullptr;
    }
    const auto existingAddon = this->GetMenuAddonByID(addon->GetID());
    if (existingAddon == nullptr)
    {
        this->m_MenuAddons.push_back(addon);
        LOG_INFO("MenuAddon with ID {} saved successfully.", addon->GetID());
        return addon;
    }
    existingAddon->SetName(addon->GetName());
    existingAddon->SetPrice(addon->GetPrice());
    for (const auto &ingredient : addon->GetIngredients())
    {
        existingAddon->AddIngredient(ingredient);
    }
    for (const auto &key : existingAddon->GetIngredientsMap() | std::views::keys)
    {
        if (!addon->GetIngredientsMap().contains(key))
        {
            existingAddon->RemoveIngredient(key);
        }
    }
    LOG_INFO("MenuAddon with ID {} updated successfully.", addon->GetID());
    return existingAddon;
}

void IMenuAddonRepository::RemoveMenuAddon(const std::shared_ptr<MenuAddon> &addon)
{
    if (addon == nullptr)
    {
        LOG_ERROR("Error removing MenuAddon: Addon is null.");
        return;
    }
    this->RemoveMenuAddon(addon->GetID());
    LOG_INFO("MenuAddon with ID {} removed successfully.", addon->GetID());
}

void IMenuAddonRepository::RemoveMenuAddon(const int AddonID)
{
    for (const auto &addon : this->m_MenuAddons)
    {
        if (addon->GetID() == AddonID)
        {
            LOG_INFO("Removing MenuAddon with ID {}.", AddonID);
            std::erase(this->m_MenuAddons, addon);
            return;
        }
    }
    LOG_ERROR("MenuAddon with ID {} not found.", AddonID);
}

std::vector<std::shared_ptr<MenuAddon>> IMenuAddonRepository::GetMenuAddons() const
{
    return this->m_MenuAddons;
}

std::shared_ptr<MenuAddon> IMenuAddonRepository::GetMenuAddonByID(int ID) const
{
    for (const auto &addon : this->m_MenuAddons)
    {
        if (addon->GetID() == ID)
        {
            return addon;
        }
    }
    return nullptr;
}

std::shared_ptr<MenuAddon> IMenuAddonRepository::GetMenuAddonByName(const std::string &Name) const
{
    for (const auto &addon : this->m_MenuAddons)
    {
        if (addon->GetName() == Name)
        {
            return addon;
        }
    }
    return nullptr;
}

std::shared_ptr<MenuItem> IMenuItemRepository::SaveMenuItem(const std::shared_ptr<MenuItem> &item)
{
    if (item == nullptr)
    {
        LOG_ERROR("Error saving MenuItem: Item is null.");
        return nullptr;
    }
    const auto existingItem = this->GetMenuItemByID(item->GetID());
    if (existingItem == nullptr)
    {
        this->m_MenuItems.push_back(item);
        return item;
    }
    existingItem->SetTitle(item->GetTitle());
    existingItem->SetDescription(item->GetDescription());
    existingItem->SetPrice(item->GetPrice());
    for (const auto &key : existingItem->GetIngredientsMap() | std::views::keys)
    {
        if (!item->GetIngredientsMap().contains(key))
        {
            existingItem->RemoveIngredient(key);
        }
    }
    for (const auto &ingredient : item->GetIngredients())
    {
        existingItem->AddIngredient(ingredient.Name, ingredient.Quantity);
    }
    for (const auto &existingAddon : existingItem->GetAvailableAddons())
    {
        if (!item->ContainsAddon(existingAddon->GetID()))
        {
            existingItem->RemoveAddon(existingAddon);
        }
    }
    for (const auto &addon : item->GetAvailableAddons())
    {
        if (!existingItem->ContainsAddon(addon->GetID()))
        {
            existingItem->AddAddon(addon);
        }
    }
    LOG_INFO("MenuItem with ID {} saved successfully.", item->GetID());
    return existingItem;
}

void IMenuItemRepository::RemoveMenuItem(const std::shared_ptr<MenuItem> &item)
{
    if (item == nullptr)
    {
        LOG_ERROR("Error removing MenuItem: Item is null.");
        return;
    }
    this->RemoveMenuItem(item->GetID());
    LOG_INFO("MenuItem with ID {} removed successfully.", item->GetID());
}

void IMenuItemRepository::RemoveMenuItem(int ItemID)
{
    for (const auto &item : this->m_MenuItems)
    {
        if (item->GetID() == ItemID)
        {
            LOG_INFO("Removing MenuItem with ID {}.", ItemID);
            std::erase(this->m_MenuItems, item);
            return;
        }
    }
    LOG_ERROR("MenuItem with ID {} not found.", ItemID);
}

std::vector<std::shared_ptr<MenuItem>> IMenuItemRepository::GetMenuItems() const
{
    return this->m_MenuItems;
}

std::shared_ptr<MenuItem> IMenuItemRepository::GetMenuItemByID(int ID) const
{
    for (const auto &item : this->m_MenuItems)
    {
        if (item->GetID() == ID)
        {
            return item;
        }
    }
    return nullptr;
}

std::shared_ptr<MenuItem> IMenuItemRepository::GetMenuItemByName(const std::string &Name) const
{
    for (const auto &item : this->m_MenuItems)
    {
        if (item->GetTitle() == Name)
        {
            return item;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Menu>> IMenuRepository::GetMenus() const
{
    return this->m_Menus;
}

std::shared_ptr<Menu> IMenuRepository::SaveMenu(const std::shared_ptr<Menu> &MenuPtr)
{
    if (MenuPtr == nullptr)
    {
        LOG_ERROR("Error saving Menu: Menu is null.");
        return nullptr;
    }
    const auto existingMenu = this->GetMenuByID(MenuPtr->GetID());
    if (existingMenu == nullptr)
    {
        this->m_Menus.push_back(MenuPtr);
        LOG_INFO("Menu with ID {} saved successfully.", MenuPtr->GetID());
        return MenuPtr;
    }
    existingMenu->SetName(MenuPtr->GetName());
    existingMenu->SetDescription(MenuPtr->GetDescription());
    for (const auto &section : MenuPtr->GetSections())
    {
        SaveSection(MenuPtr, section);
    }
    LOG_INFO("Menu with ID {} updated successfully.", MenuPtr->GetID());
    return existingMenu;
}

void IMenuRepository::SaveSection(const std::shared_ptr<Menu> &MenuPtr, const std::shared_ptr<MenuSection> &Section)
{
    if (Section == nullptr)
    {
        LOG_ERROR("Error saving MenuSection: Section is null.");
        return;
    }
    if (MenuPtr == nullptr)
    {
        LOG_ERROR("Error saving MenuSection: Menu is null.");
        return;
    }
    const auto existingSection = MenuPtr->GetSectionByID(Section->GetID());
    if (existingSection == nullptr)
    {
        MenuPtr->AddSection(Section);
        LOG_INFO("MenuSection with ID {} added to Menu with ID {}.", Section->GetID(), MenuPtr->GetID());
        return;
    }
    existingSection->SetTitle(Section->GetTitle());
    existingSection->SetDescription(Section->GetDescription());
    for (const auto &item : existingSection->GetMenuItems())
    {
        if (Section->ContainsItem(item->GetID()) == false)
        {
            existingSection->RemoveItem(item->GetID());
        }
    }
    for (const auto &item : Section->GetMenuItems())
    {
        if (existingSection->ContainsItem(item->GetID()) == false)
        {
            existingSection->AddMenuItem(item);
        }
    }
    LOG_INFO("MenuSection with ID {} updated in Menu with ID {}.", Section->GetID(), MenuPtr->GetID());
}

void IMenuRepository::RemoveMenu(const std::shared_ptr<Menu> &Menu)
{
    if (Menu == nullptr)
    {
        LOG_ERROR("Error removing Menu: Menu is null.");
        return;
    }
    this->RemoveMenu(Menu->GetID());
}

void IMenuRepository::RemoveMenu(const int MenuID)
{
    for (const auto &menu : this->m_Menus)
    {
        if (menu->GetID() == MenuID)
        {
            LOG_INFO("Removing Menu with ID {}.", MenuID);
            std::erase(this->m_Menus, menu);
            return;
        }
    }
    LOG_ERROR("Menu with ID {} not found.", MenuID);
}

std::shared_ptr<Menu> IMenuRepository::GetMenuByID(const int MenuID) const
{
    for (const auto &menu : this->m_Menus)
    {
        if (menu->GetID() == MenuID)
        {
            return menu;
        }
    }
    return nullptr;
}

std::shared_ptr<Menu> IMenuRepository::GetMenuByName(const std::string &Name) const
{
    for (const auto &menu : this->m_Menus)
    {
        if (menu->GetName() == Name)
        {
            return menu;
        }
    }
    return nullptr;
}
