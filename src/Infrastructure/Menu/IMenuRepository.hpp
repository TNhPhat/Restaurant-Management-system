#pragma once

#include <vector>
#include <memory>
#include <string>
#include "Menu/Menu.hpp"

class IMenuRepository {
public:
    virtual ~IMenuRepository() = default;

    virtual void SaveMenus(std::string filePath) const = 0;

    virtual void SaveItems(std::string filePath) const = 0;

    virtual void SaveAddons(std::string filePath) const = 0;

    std::shared_ptr<MenuAddon> SaveMenuAddon(const std::shared_ptr<MenuAddon> &addon);

    void RemoveMenuAddon(int AddonID);

    std::shared_ptr<MenuAddon> GetMenuAddonByID(int ID) const;

    std::vector<std::shared_ptr<MenuAddon> > GetMenuAddons() const;

    std::shared_ptr<MenuItem> SaveMenuItem(const std::shared_ptr<MenuItem> &item);

    std::shared_ptr<MenuItem> GetMenuItemByID(int ID) const;

    void RemoveMenuItem(int ItemID);

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;

    std::vector<std::shared_ptr<Menu> > GetMenus() const;

    std::shared_ptr<Menu> SaveMenu(const std::shared_ptr<Menu> &MenuPtr);

    void SaveSection(const std::shared_ptr<Menu> &MenuPtr, const std::shared_ptr<MenuSection> &Section);

    void RemoveMenu(const std::shared_ptr<Menu> &Menu);

    void RemoveMenu(int MenuID);

    std::shared_ptr<Menu> GetMenuByID(int MenuID) const;

    std::vector<std::shared_ptr<MenuSection> > GetSections();

    std::shared_ptr<MenuSection> GetSectionByID(int SectionID) const;

    void RemoveSection(int SectionID);

    void SaveSection(const std::shared_ptr<MenuSection> &Section);

private:
    std::vector<std::shared_ptr<Menu> > m_Menus;
    std::vector<std::shared_ptr<MenuSection> > m_Sections;
    std::vector<std::shared_ptr<MenuItem> > m_Items;
    std::vector<std::shared_ptr<MenuAddon> > m_Addons;
};
