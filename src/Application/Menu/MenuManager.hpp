#pragma once
#include "../../Infrastructure/Menu/FileMenuRepository.hpp"

class MenuAddonManager {
private:
    FileMenuAddonRepository &m_MenuAddonRepository;

public:
    MenuAddonManager(FileMenuAddonRepository &MenuAddonRepository);

    std::shared_ptr<MenuAddon> GetMenuAddonByID(int MenuAddonID) const;

    std::shared_ptr<MenuAddon> GetMenuAddonByName(std::string MenuAddonName) const;

    std::vector<std::shared_ptr<MenuAddon> > GetMenuAddons() const;

    std::shared_ptr<MenuAddon> SaveMenuAddon(const std::shared_ptr<MenuAddon> &MenuAddonPtr) const;

    void RemoveMenuAddon(const std::shared_ptr<MenuAddon> &MenuAddon);

    void RemoveMenuAddon(int MenuAddonID);

    ~MenuAddonManager() = default;
};

class MenuItemManager {
private:
    FileMenuItemRepository &m_MenuItemRepository;

public:
    MenuItemManager(FileMenuItemRepository &MenuItemRepository);

    std::shared_ptr<MenuItem> GetMenuItemByID(int MenuItemID) const;

    std::shared_ptr<MenuItem> GetMenuItemByName(const std::string &MenuItemName) const;

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;

    std::shared_ptr<MenuItem> SaveMenuItem(const std::shared_ptr<MenuItem> &MenuItemPtr) const;

    void RemoveMenuItem(const std::shared_ptr<MenuItem> &MenuItem);

    void RemoveMenuItem(int MenuItemID);

    std::vector<std::shared_ptr<MenuAddon> > GetAvailableAddonsForItem(int MenuItemID) const;


    ~MenuItemManager() = default;
};

class MenuManager {
private:
    FileMenuRepository &m_MenuRepository;

public:
    MenuManager(FileMenuRepository &MenuRepository);

    std::shared_ptr<Menu> GetMenuByID(int MenuID) const;

    std::shared_ptr<Menu> GetMenuByName(const std::string &Name) const;

    std::vector<std::shared_ptr<Menu> > GetMenus() const;

    std::shared_ptr<Menu> SaveMenu(const std::shared_ptr<Menu> &MenuPtr) const;

    void RemoveMenu(const std::shared_ptr<Menu> &Menu);

    void RemoveMenu(const int MenuID);

    ~MenuManager() = default;
};
