#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Menu/Menu.hpp"

class IMenuAddonRepository {
public:
    virtual ~IMenuAddonRepository() = default;

    virtual void SaveMenuAddons(std::string filePath) const = 0;

    std::shared_ptr<MenuAddon> SaveMenuAddon(const std::shared_ptr<MenuAddon> &addon);

    void RemoveMenuAddon(const std::shared_ptr<MenuAddon> &addon);

    void RemoveMenuAddon(int AddonID);

    std::vector<std::shared_ptr<MenuAddon> > GetMenuAddons() const;

    std::shared_ptr<MenuAddon> GetMenuAddonByID(int ID) const;

    std::shared_ptr<MenuAddon> GetMenuAddonByName(const std::string &Name) const;

private:
    std::vector<std::shared_ptr<MenuAddon> > m_MenuAddons;
};

class IMenuItemRepository {
public:
    virtual ~IMenuItemRepository() = default;

    virtual void SaveMenuItems(std::string filePath) const = 0;

    std::shared_ptr<MenuItem> SaveMenuItem(const std::shared_ptr<MenuItem> &item);

    void RemoveMenuItem(const std::shared_ptr<MenuItem> &item);

    void RemoveMenuItem(int ItemID);

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;

    std::shared_ptr<MenuItem> GetMenuItemByID(int ID) const;

    std::shared_ptr<MenuItem> GetMenuItemByName(const std::string &Name) const;

public:
    std::vector<std::shared_ptr<MenuItem> > m_MenuItems;
};

class IMenuRepository {
public:
    virtual ~IMenuRepository() = default;

    virtual void SaveMenu(std::string filePath) const = 0;

    std::vector<std::shared_ptr<Menu> > GetMenus() const;

    std::shared_ptr<Menu> SaveMenu(const std::shared_ptr<Menu> &MenuPtr);

    void SaveSection(const std::shared_ptr<Menu> &MenuPtr, const std::shared_ptr<MenuSection> &Section);

    void RemoveMenu(const std::shared_ptr<Menu> &Menu);

    void RemoveMenu(const int MenuID);

    std::shared_ptr<Menu> GetMenuByID(const int MenuID) const;

    std::shared_ptr<Menu> GetMenuByName(const std::string &Name) const;

private:
    std::vector<std::shared_ptr<Menu> > m_Menus;
};
