#pragma once
#include "Screen.hpp"
#include "FileMenuRepository.hpp"
#include "Application/Menu/MenuManager.hpp"

class MenuScreen : public Screen {
    static FileMenuAddonRepository menuAddonRepository;
    static MenuAddonManager menuAddonManager;
    static FileMenuItemRepository menuItemRepository;
    static MenuItemManager menuItemManager;
    static FileMenuRepository menuRepository;
    static MenuManager menuManager;

public:
    MenuScreen(Core &core);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;
};
