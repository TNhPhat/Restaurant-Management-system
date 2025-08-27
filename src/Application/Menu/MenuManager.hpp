#pragma once
#include "IMenuRepository.hpp"

class MenuManager {
private:
    std::unique_ptr<IMenuRepository> m_MenuRepository = nullptr;

public:
    MenuManager(IMenuRepository *MenuRepository);

    std::shared_ptr<Menu> GetMenuByID(int MenuID) const;

    std::vector<std::shared_ptr<Menu> > GetMenus() const;

    std::shared_ptr<Menu> SaveMenu(const std::shared_ptr<Menu> &MenuPtr) const;

    void RemoveMenu(const int MenuID);

    std::shared_ptr<MenuItem> GetMenuItemByID(int MenuItemID) const;

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;

    std::shared_ptr<MenuItem> SaveMenuItem(const std::shared_ptr<MenuItem> &MenuItemPtr) const;

    void RemoveMenuItem(int MenuItemID);

    std::vector<std::shared_ptr<MenuAddon> > GetAvailableAddonsForItem(int MenuItemID) const;

    std::shared_ptr<MenuAddon> GetMenuAddonByID(int MenuAddonID) const;

    std::vector<std::shared_ptr<MenuAddon> > GetMenuAddons() const;

    std::shared_ptr<MenuAddon> SaveMenuAddon(const std::shared_ptr<MenuAddon> &MenuAddonPtr) const;

    void RemoveMenuAddon(int MenuAddonID);

    std::vector<std::shared_ptr<MenuSection> > GetSections() const;

    std::shared_ptr<MenuSection> GetSectionByID(int SectionID) const;

    void RemoveSection(int SectionID);

    ~MenuManager();

    void SaveSection(const std::shared_ptr<MenuSection> &shared);
};
