#include "MenuManager.hpp"

MenuAddonManager::MenuAddonManager(FileMenuAddonRepository &MenuAddonRepository): m_MenuAddonRepository(
    MenuAddonRepository) {
}

MenuItemManager::MenuItemManager(FileMenuItemRepository &MenuItemRepository): m_MenuItemRepository(MenuItemRepository) {
}

MenuManager::MenuManager(FileMenuRepository &MenuRepository): m_MenuRepository(MenuRepository) {
}
