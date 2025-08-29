#include "IMenuRepository.hpp"

std::shared_ptr<MenuAddon> IMenuRepository::SaveMenuAddon(const std::shared_ptr<MenuAddon> &addon) {
    if (addon == nullptr) {
        LOG_ERROR("Error saving MenuAddon: Addon is null.");
        return nullptr;
    }
    const auto existingAddon = this->GetMenuAddonByID(addon->GetID());
    if (existingAddon == nullptr) {
        this->m_Addons.push_back(addon);
        return addon;
    }
    existingAddon->SetName(addon->GetName());
    existingAddon->SetPrice(addon->GetPrice());
    for (const auto &ingredient: addon->GetIngredients()) {
        existingAddon->AddIngredient(ingredient);
    }
    for (const auto &key: existingAddon->GetIngredientsMap() | std::views::keys) {
        if (!addon->GetIngredientsMap().contains(key)) {
            existingAddon->RemoveIngredient(key);
        }
    }
    LOG_INFO("MenuAddon with ID {} updated successfully.", addon->GetID());
    return existingAddon;
}

void IMenuRepository::RemoveMenuAddon(const int AddonID) {
    for (const auto &addon: this->m_Addons) {
        if (addon->GetID() == AddonID) {
            for (auto &item: this->m_Items) {
                item->RemoveAddon(addon);
            }
            LOG_INFO("Removing MenuAddon with ID {}.", AddonID);
            std::erase(this->m_Addons, addon);
            return;
        }
    }
    LOG_ERROR("MenuAddon with ID {} not found.", AddonID);
}

std::vector<std::shared_ptr<MenuAddon> > IMenuRepository::GetMenuAddons() const {
    return this->m_Addons;
}

std::shared_ptr<MenuAddon> IMenuRepository::GetMenuAddonByID(int ID) const {
    for (const auto &addon: this->m_Addons) {
        if (addon->GetID() == ID) {
            return addon;
        }
    }
    return nullptr;
}

std::shared_ptr<MenuItem> IMenuRepository::SaveMenuItem(const std::shared_ptr<MenuItem> &item) {
    if (item == nullptr) {
        LOG_ERROR("Error saving MenuItem: Item is null.");
        return nullptr;
    }
    const auto existingItem = this->GetMenuItemByID(item->GetID());
    if (existingItem == nullptr) {
        this->m_Items.push_back(item);
        return item;
    }
    existingItem->SetTitle(item->GetTitle());
    existingItem->SetDescription(item->GetDescription());
    existingItem->SetPrice(item->GetPrice());
    for (const auto &key: existingItem->GetIngredientsMap() | std::views::keys) {
        if (!item->GetIngredientsMap().contains(key)) {
            existingItem->RemoveIngredient(key);
        }
    }
    for (const auto &ingredient: item->GetIngredients()) {
        existingItem->AddIngredient(ingredient.Name, ingredient.Quantity);
    }
    for (const auto &existingAddon: existingItem->GetAvailableAddons()) {
        if (!item->ContainsAddon(existingAddon->GetID())) {
            existingItem->RemoveAddon(existingAddon);
        }
    }
    for (const auto &addon: item->GetAvailableAddons()) {
        if (!existingItem->ContainsAddon(addon->GetID())) {
            existingItem->AddAddon(addon);
        }
    }
    LOG_INFO("MenuItem with ID {} saved successfully.", item->GetID());
    return existingItem;
}

void IMenuRepository::RemoveMenuItem(int ItemID) {
    for (const auto &item: this->m_Items) {
        if (item->GetID() == ItemID) {
            LOG_INFO("Removing MenuItem with ID {}.", ItemID);
            for (auto &section: this->m_Sections) {
                section->RemoveItem(ItemID);
            }
            std::erase(this->m_Items, item);
            return;
        }
    }
    LOG_ERROR("MenuItem with ID {} not found.", ItemID);
}

std::vector<std::shared_ptr<MenuItem> > IMenuRepository::GetMenuItems() const {
    return this->m_Items;
}

std::shared_ptr<MenuItem> IMenuRepository::GetMenuItemByID(int ID) const {
    for (const auto &item: this->m_Items) {
        if (item->GetID() == ID) {
            return item;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<Menu> > IMenuRepository::GetMenus() const {
    return this->m_Menus;
}

std::shared_ptr<Menu> IMenuRepository::SaveMenu(const std::shared_ptr<Menu> &MenuPtr) {
    if (MenuPtr == nullptr) {
        LOG_ERROR("Error saving Menu: Menu is null.");
        return nullptr;
    }
    const auto existingMenu = this->GetMenuByID(MenuPtr->GetID());
    if (existingMenu == nullptr) {
        this->m_Menus.push_back(MenuPtr);
        LOG_INFO("Menu with ID {} saved successfully.", MenuPtr->GetID());
        return MenuPtr;
    }
    existingMenu->SetName(MenuPtr->GetName());
    existingMenu->SetDescription(MenuPtr->GetDescription());
    for (const auto &section: MenuPtr->GetSections()) {
        SaveSection(MenuPtr, section);
    }
    LOG_INFO("Menu with ID {} updated successfully.", MenuPtr->GetID());
    return existingMenu;
}

void IMenuRepository::SaveSection(const std::shared_ptr<Menu> &MenuPtr, const std::shared_ptr<MenuSection> &Section) {
    if (Section == nullptr) {
        LOG_ERROR("Error saving MenuSection: Section is null.");
        return;
    }
    if (MenuPtr == nullptr) {
        LOG_ERROR("Error saving MenuSection: Menu is null.");
        return;
    }
    const auto sectionInRepo = this->GetSectionByID(Section->GetID());
    if (sectionInRepo == nullptr) {
        this->m_Sections.push_back(Section);
    }
    const auto existingSection = MenuPtr->GetSectionByID(Section->GetID());
    if (existingSection == nullptr) {
        MenuPtr->AddSection(Section);
        LOG_INFO("MenuSection with ID {} added to Menu with ID {}.", Section->GetID(), MenuPtr->GetID());
        return;
    }
    existingSection->SetTitle(Section->GetTitle());
    existingSection->SetDescription(Section->GetDescription());
    for (const auto &item: existingSection->GetMenuItems()) {
        if (Section->ContainsItem(item->GetID()) == false) {
            existingSection->RemoveItem(item->GetID());
        }
    }
    for (const auto &item: Section->GetMenuItems()) {
        if (existingSection->ContainsItem(item->GetID()) == false) {
            existingSection->AddMenuItem(item);
        }
    }
    LOG_INFO("MenuSection with ID {} updated in Menu with ID {}.", Section->GetID(), MenuPtr->GetID());
}

void IMenuRepository::RemoveMenu(const std::shared_ptr<Menu> &Menu) {
    if (Menu == nullptr) {
        LOG_ERROR("Error removing Menu: Menu is null.");
        return;
    }
    this->RemoveMenu(Menu->GetID());
}

void IMenuRepository::RemoveMenu(const int MenuID) {
    for (const auto &menu: this->m_Menus) {
        if (menu->GetID() == MenuID) {
            LOG_INFO("Removing Menu with ID {}.", MenuID);
            std::erase(this->m_Menus, menu);
            return;
        }
    }
    LOG_ERROR("Menu with ID {} not found.", MenuID);
}

std::shared_ptr<Menu> IMenuRepository::GetMenuByID(const int MenuID) const {
    for (const auto &menu: this->m_Menus) {
        if (menu->GetID() == MenuID) {
            return menu;
        }
    }
    return nullptr;
}

std::vector<std::shared_ptr<MenuSection> > IMenuRepository::GetSections() {
    return this->m_Sections;
}

std::shared_ptr<MenuSection> IMenuRepository::GetSectionByID(int SectionID) const {
    for (const auto &section: this->m_Sections) {
        if (section->GetID() == SectionID) {
            return section;
        }
    }
    return nullptr;
}

void IMenuRepository::RemoveSection(int SectionID) {
    for (const auto &section: this->m_Sections) {
        if (section->GetID() == SectionID) {
            for (auto &menu: this->m_Menus) {
                menu->RemoveSection(SectionID);
            }
            LOG_INFO("Removing MenuSection with ID {}.", SectionID);
            std::erase(this->m_Sections, section);
            return;
        }
    }
    LOG_ERROR("MenuSection with ID {} not found.", SectionID);
}

void IMenuRepository::SaveSection(const std::shared_ptr<MenuSection> &Section) {
    if (Section == nullptr) {
        LOG_ERROR("Error saving MenuSection: Section is null.");
        return;
    }
    const auto sectionInRepo = this->GetSectionByID(Section->GetID());
    if (sectionInRepo == nullptr) {
        this->m_Sections.push_back(Section);
        this->GetMenuByID(Section->GetMenuID())->AddSection(Section);
        return;
    }
    sectionInRepo->SetTitle(Section->GetTitle());
    sectionInRepo->SetDescription(Section->GetDescription());
    for (const auto &item: sectionInRepo->GetMenuItems()) {
        if (Section->ContainsItem(item->GetID()) == false) {
            sectionInRepo->RemoveItem(item->GetID());
        }
    }
    for (const auto &item: Section->GetMenuItems()) {
        if (sectionInRepo->ContainsItem(item->GetID()) == false) {
            sectionInRepo->AddMenuItem(item);
        }
    }
    LOG_INFO("MenuSection with ID {} updated.", Section->GetID());
}
