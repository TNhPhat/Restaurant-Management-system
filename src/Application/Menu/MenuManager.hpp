#pragma once
#include "Menu/Menu.hpp"

class MenuAddonManager {
private:
    MenuAddonManager();

    ~MenuAddonManager() = default;

    std::vector<std::shared_ptr<MenuAddon> > m_MenuAddons;
    std::unique_ptr<JsonHandle> m_FileHandle;
    bool m_IsLoaded;

public:
    MenuAddonManager(const MenuAddonManager &) = delete;

    MenuAddonManager &operator=(const MenuAddonManager &) = delete;

    static MenuAddonManager &GetInstance();

    void LoadMenuAddonsFromFile(const std::string &FilePath);

    void SaveMenuAddonsToFile(const std::string &FilePath) const;

    std::vector<std::shared_ptr<MenuAddon> > GetMenuAddons() const;

    std::shared_ptr<MenuAddon> GetMenuAddonByID(int ID) const;

    std::shared_ptr<MenuAddon> GetMenuAddonByName(const std::string &Name) const;

    void AddMenuAddon(const std::shared_ptr<MenuAddon> &Addon);

    void RemoveMenuAddon(const std::shared_ptr<MenuAddon> &Addon);

    bool IsLoaded() const;
};

class MenuItemManager {
private:
    MenuItemManager();

    ~MenuItemManager() = default;

    std::vector<std::shared_ptr<MenuItem> > m_MenuItems;
    std::unique_ptr<JsonHandle> m_FileHandle;
    bool m_IsLoaded;

public:
    MenuItemManager(const MenuItemManager &) = delete;

    MenuItemManager &operator=(const MenuItemManager &) = delete;

    static MenuItemManager &GetInstance();

    void LoadMenuItemsFromFile(const std::string &FilePath);

    void SaveMenuItemsToFile(const std::string &FilePath) const;

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;

    void AddMenuItem(const std::shared_ptr<MenuItem> &Item);

    void RemoveMenuItem(int ItemID);

    std::shared_ptr<MenuItem> GetMenuItemByID(int ItemID) const;

    std::shared_ptr<MenuItem> GetMenuItemByName(const std::string &Name) const;

    bool IsLoaded() const;
};

class MenuManager {
private:
    MenuManager();

    ~MenuManager() = default;

    std::unique_ptr<JsonHandle> m_FileHandle;
    std::vector<std::shared_ptr<Menu> > m_MenuItems;
    bool m_IsLoaded;

public:
    MenuManager(const MenuManager &) = delete;

    MenuManager &operator=(const MenuManager &) = delete;

    static MenuManager &GetInstance();

    void LoadMenuFromFile(const std::string &FilePath);

    void SaveMenuToFile(const std::string &FilePath) const;

    std::vector<std::shared_ptr<Menu> > GetMenus() const;

    void AddMenu(const std::shared_ptr<Menu> &Menu);

    void RemoveMenu(int MenuID);

    std::shared_ptr<Menu> GetMenuByID(int MenuID) const;

    std::shared_ptr<Menu> GetMenuByName(const std::string &Name) const;

    bool IsLoaded() const;
};
