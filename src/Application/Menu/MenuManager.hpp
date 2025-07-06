#pragma once
#include "../../Infrastructure/Menu/FileMenuRepository.hpp"

class MenuAddonManager {
private:
    FileMenuAddonRepository &m_MenuAddonRepository;

public:
    MenuAddonManager(FileMenuAddonRepository &MenuAddonRepository);

    ~MenuAddonManager() = default;
};

class MenuItemManager {
private:
    FileMenuItemRepository &m_MenuItemRepository;

public:
    MenuItemManager(FileMenuItemRepository &MenuItemRepository);

    ~MenuItemManager() = default;
};

class MenuManager {
private:
    FileMenuRepository &m_MenuRepository;

public:
    MenuManager(FileMenuRepository &MenuRepository);

    ~MenuManager() = default;
};
