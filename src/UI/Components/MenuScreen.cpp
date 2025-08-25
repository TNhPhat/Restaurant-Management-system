#include "MenuScreen.hpp"

MenuScreen::MenuScreen(Core &core): Screen(core) {
}

void MenuScreen::Render(float DT) {
    const auto &menu = menuManager.GetMenus();
    for (auto &m: menu) {
        ImGui::Text("Menu: %s", m->GetName().c_str());
        ImGui::Separator();
        for (const auto &item: m->GetSections()) {
            ImGui::Text("Section: %s", item->GetTitle().c_str());
            ImGui::Separator();
            for (const auto &menuItem: item->GetMenuItems()) {
                ImGui::Text("Item: %s", menuItem->GetTitle().c_str());
                ImGui::Text("Price: %.2f", menuItem->GetPrice());
                ImGui::Text("Description: %s", menuItem->GetDescription().c_str());
                ImGui::Separator();
            }
            ImGui::Text("End of Section: %s", item->GetTitle().c_str());
            ImGui::Separator();
        }
    }
}

FileMenuAddonRepository MenuScreen::menuAddonRepository("Data/MenuAddons.json");
MenuAddonManager MenuScreen::menuAddonManager(menuAddonRepository);
FileMenuItemRepository MenuScreen::menuItemRepository("Data/MenuItem.json", menuAddonRepository);
MenuItemManager MenuScreen::menuItemManager(menuItemRepository);
FileMenuRepository MenuScreen::menuRepository("Data/Menu.json", menuItemRepository);
MenuManager MenuScreen::menuManager(menuRepository);

void MenuScreen::Init() {
}

void MenuScreen::OnExit() {
}
