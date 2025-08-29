#include "MenuManager.hpp"

#include <utility>

std::shared_ptr<MenuAddon> MenuManager::GetMenuAddonByID(int MenuAddonID) const {
    return this->m_MenuRepository->GetMenuAddonByID(MenuAddonID);
}

std::vector<std::shared_ptr<MenuAddon> > MenuManager::GetMenuAddons() const {
    return this->m_MenuRepository->GetMenuAddons();
}

std::shared_ptr<MenuAddon> MenuManager::SaveMenuAddon(const std::shared_ptr<MenuAddon> &MenuAddonPtr) const {
    return this->m_MenuRepository->SaveMenuAddon(MenuAddonPtr);
}

void MenuManager::RemoveMenuAddon(int MenuAddonID) {
    return this->m_MenuRepository->RemoveMenuAddon(MenuAddonID);
}

std::vector<std::shared_ptr<MenuSection> > MenuManager::GetSections() const {
    return this->m_MenuRepository->GetSections();
}

std::shared_ptr<MenuSection> MenuManager::GetSectionByID(int SectionID) const {
    return this->m_MenuRepository->GetSectionByID(SectionID);
}

void MenuManager::RemoveSection(int SectionID) {
    return this->m_MenuRepository->RemoveSection(SectionID);
}

MenuManager::~MenuManager() {
    this->m_MenuRepository->SaveMenus("Data/Menu.json");
    this->m_MenuRepository->SaveItems("Data/MenuItem.json");
    this->m_MenuRepository->SaveAddons("Data/MenuAddons.json");
}

void MenuManager::SaveSection(const std::shared_ptr<MenuSection> &shared) {
    return this->m_MenuRepository->SaveSection(shared);
}

std::shared_ptr<const IMenuRepository> MenuManager::GetMenuRepository() const {
    return const_pointer_cast<const IMenuRepository, IMenuRepository>(this->m_MenuRepository);
}

std::shared_ptr<MenuItem> MenuManager::GetMenuItemByID(int MenuItemID) const {
    return this->m_MenuRepository->GetMenuItemByID(MenuItemID);
}


std::vector<std::shared_ptr<MenuItem> > MenuManager::GetMenuItems() const {
    return this->m_MenuRepository->GetMenuItems();
}

std::shared_ptr<MenuItem> MenuManager::SaveMenuItem(const std::shared_ptr<MenuItem> &MenuItemPtr) const {
    return this->m_MenuRepository->SaveMenuItem(MenuItemPtr);
}

void MenuManager::RemoveMenuItem(int MenuItemID) {
    return this->m_MenuRepository->RemoveMenuItem(MenuItemID);
}

std::vector<std::shared_ptr<MenuAddon> > MenuManager::GetAvailableAddonsForItem(const int MenuItemID) const {
    return this->m_MenuRepository->GetMenuItemByID(MenuItemID)->GetAvailableAddons();
}

MenuManager::MenuManager(std::shared_ptr<IMenuRepository> MenuRepository): m_MenuRepository(std::move(MenuRepository)) {
}

std::shared_ptr<Menu> MenuManager::GetMenuByID(const int MenuID) const {
    return this->m_MenuRepository->GetMenuByID(MenuID);
}


std::vector<std::shared_ptr<Menu> > MenuManager::GetMenus() const {
    return this->m_MenuRepository->GetMenus();
}

std::shared_ptr<Menu> MenuManager::SaveMenu(const std::shared_ptr<Menu> &MenuPtr) const {
    return this->m_MenuRepository->SaveMenu(MenuPtr);
}

void MenuManager::RemoveMenu(const int MenuID) {
    return this->m_MenuRepository->RemoveMenu(MenuID);
}
