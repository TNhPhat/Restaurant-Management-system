#pragma once
#include "Screen.hpp"
#include "imgui_internal.h"
#include "Application/Menu/MenuManager.hpp"
#include "../../Core/Utils/CurrencyUtils.hpp"

template<typename T>
class SubMenuScreen {
protected:
    virtual bool RenderLeft(T &instance) = 0;

    virtual void RenderRight(T &instance) = 0;

    bool m_isTabSelected = false;
    int m_CurrentChoiceID = -1;
    int m_SelectedID = -1;
    std::string m_ID;
    bool m_shouldRefresh = true;

    // Filter components
    ImGuiTextFilter m_filter;

public:
    virtual ~SubMenuScreen() = default;

    bool Render(T &instance);
};

template<typename T>
bool SubMenuScreen<T>::Render(T &instance) {
    if (ImGui::BeginTabItem(m_ID.c_str(), nullptr, ImGuiTabItemFlags_None)) {
        bool ret = RenderLeft(instance);
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
        ImGui::SameLine();
        RenderRight(instance);
        ImGui::EndTabItem();
        return ret;
    }
    return false;
}

class MenuSubMenuScreen : public SubMenuScreen<MenuManager> {
private:
    friend class MenuScreen;
    std::vector<std::shared_ptr<Menu> > m_currentMenus;
    std::string m_nameField, m_descriptionField;

public:
    MenuSubMenuScreen();

    bool RenderLeft(MenuManager &instance) override;

    void RenderRight(MenuManager &instance) override;
};

class SectionSubMenuScreen : public SubMenuScreen<MenuManager> {
private:
    std::vector<std::shared_ptr<MenuSection> > m_currentSections;
    std::string m_nameField, m_descriptionField;

public:
    friend class MenuScreen;

    SectionSubMenuScreen();

    bool RenderLeft(MenuManager &instance) override;

    void RenderRight(MenuManager &instance) override;
};

class ItemSubMenuScreen : public SubMenuScreen<MenuManager> {
private:
    std::vector<std::shared_ptr<MenuItem> > m_currentItems;
    std::string m_nameField, m_descriptionField;
    float m_priceField = 0.0f;
    std::map<std::string, int> m_ingredientsField;
    std::map<std::string, bool> m_deletedField;

public:
    friend class MenuScreen;

    ItemSubMenuScreen();

    bool RenderLeft(MenuManager &instance) override;

    void RenderRight(MenuManager &instance) override;
};

class AddonSubMenuScreen : public SubMenuScreen<MenuManager> {
private:
    std::vector<std::shared_ptr<MenuAddon> > m_currentAddons;
    std::string m_nameField;
    float m_priceField = 0.0f;
    std::map<std::string, int> m_ingredientsField;
    std::map<std::string, bool> m_deletedField;

public:
    friend class MenuScreen;

    AddonSubMenuScreen();

    bool RenderLeft(MenuManager &instance) override;

    void RenderRight(MenuManager &instance) override;
};

class MenuScreen : public Screen {
    MenuManager s_menuManager;
    MenuSubMenuScreen m_menuSubMenuScreen;
    SectionSubMenuScreen m_sectionSubMenuScreen;
    ItemSubMenuScreen m_itemSubMenuScreen;
    AddonSubMenuScreen m_addonSubMenuScreen;

public:
    MenuScreen(Core &core);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;
};
