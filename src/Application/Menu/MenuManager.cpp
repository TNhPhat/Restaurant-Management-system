#include "MenuManager.hpp"

MenuAddonManager::MenuAddonManager(FileMenuAddonRepository &MenuAddonRepository): m_MenuAddonRepository(
    MenuAddonRepository) {
}

std::shared_ptr<MenuAddon> MenuAddonManager::GetMenuAddonByID(int MenuAddonID) const {
    return this->m_MenuAddonRepository.GetMenuAddonByID(MenuAddonID);
}

std::shared_ptr<MenuAddon> MenuAddonManager::GetMenuAddonByName(std::string MenuAddonName) const {
    return this->m_MenuAddonRepository.GetMenuAddonByName(MenuAddonName);
}

std::vector<std::shared_ptr<MenuAddon> > MenuAddonManager::GetMenuAddons() const {
    return this->m_MenuAddonRepository.GetMenuAddons();
}

std::shared_ptr<MenuAddon> MenuAddonManager::SaveMenuAddon(const std::shared_ptr<MenuAddon> &MenuAddonPtr) const {
    return this->m_MenuAddonRepository.SaveMenuAddon(MenuAddonPtr);
}

void MenuAddonManager::RemoveMenuAddon(const std::shared_ptr<MenuAddon> &MenuAddon) {
    return this->m_MenuAddonRepository.RemoveMenuAddon(MenuAddon);
}

void MenuAddonManager::RemoveMenuAddon(int MenuAddonID) {
    return this->m_MenuAddonRepository.RemoveMenuAddon(MenuAddonID);
}

MenuItemManager::MenuItemManager(FileMenuItemRepository &MenuItemRepository): m_MenuItemRepository(MenuItemRepository) {
}

std::shared_ptr<MenuItem> MenuItemManager::GetMenuItemByID(int MenuItemID) const {
    return this->m_MenuItemRepository.GetMenuItemByID(MenuItemID);
}

std::shared_ptr<MenuItem> MenuItemManager::GetMenuItemByName(const std::string &MenuItemName) const {
    return this->m_MenuItemRepository.GetMenuItemByName(MenuItemName);
}

std::vector<std::shared_ptr<MenuItem> > MenuItemManager::GetMenuItems() const {
    return this->m_MenuItemRepository.GetMenuItems();
}

std::shared_ptr<MenuItem> MenuItemManager::SaveMenuItem(const std::shared_ptr<MenuItem> &MenuItemPtr) const {
    return this->m_MenuItemRepository.SaveMenuItem(MenuItemPtr);
}

void MenuItemManager::RemoveMenuItem(const std::shared_ptr<MenuItem> &MenuItem) {
    return this->m_MenuItemRepository.RemoveMenuItem(MenuItem);
}

void MenuItemManager::RemoveMenuItem(int MenuItemID) {
    return this->m_MenuItemRepository.RemoveMenuItem(MenuItemID);
}

std::vector<std::shared_ptr<MenuAddon> > MenuItemManager::GetAvailableAddonsForItem(const int MenuItemID) const {
    return this->m_MenuItemRepository.GetMenuItemByID(MenuItemID)->GetAvailableAddons();
}

MenuManager::MenuManager(FileMenuRepository &MenuRepository): m_MenuRepository(MenuRepository) {
}

std::shared_ptr<Menu> MenuManager::GetMenuByID(const int MenuID) const {
    return this->m_MenuRepository.GetMenuByID(MenuID);
}

std::shared_ptr<Menu> MenuManager::GetMenuByName(const std::string &Name) const {
    return this->m_MenuRepository.GetMenuByName(Name);
}

std::vector<std::shared_ptr<Menu> > MenuManager::GetMenus() const {
    return this->m_MenuRepository.GetMenus();
}

std::shared_ptr<Menu> MenuManager::SaveMenu(const std::shared_ptr<Menu> &MenuPtr) const {
    return this->m_MenuRepository.SaveMenu(MenuPtr);
}

void MenuManager::RemoveMenu(const std::shared_ptr<Menu> &Menu) {
    return this->m_MenuRepository.RemoveMenu(Menu);
}

void MenuManager::RemoveMenu(const int MenuID) {
    return this->m_MenuRepository.RemoveMenu(MenuID);
}
