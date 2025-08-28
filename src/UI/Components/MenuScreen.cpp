#include "MenuScreen.hpp"

#include "Constants.hpp"
#include "FileMenuRepository.hpp"
#include "Application/Storage/StorageManager.hpp"
#include "UI/UICore/Core.hpp"
#include "misc/cpp/imgui_stdlib.h"

MenuScreen::MenuScreen(Core &core, std::shared_ptr<MenuManager> menuManager): Screen(core),
                                                                              s_menuManager(menuManager) {
}

void MenuScreen::Render(float DT) {
    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Menu Screen").x) / 2);
    ImGui::GetFont()->Scale *= 2.0f;
    ImGui::PushFont(ImGui::GetFont());
    ImGui::Text("Menu Screen");
    ImGui::PopFont();
    ImGui::GetFont()->Scale /= 2.0f;
    if (ImGui::Button("Go Back")) {
        m_Core.PopScreen();
    }
    ImGui::Separator();
    ImGui::BeginTabBar("Menu Tab Bar", ImGuiTabBarFlags_None);
    m_menuSubMenuScreen.Render(s_menuManager);
    m_sectionSubMenuScreen.Render(s_menuManager);
    m_itemSubMenuScreen.Render(s_menuManager);
    m_addonSubMenuScreen.Render(s_menuManager);
    ImGui::EndTabBar();
}

void MenuScreen::Init() {
}

void MenuScreen::OnExit() {
}

MenuSubMenuScreen::MenuSubMenuScreen() {
    m_ID = "MenuSubMenu";
}

bool MenuSubMenuScreen::RenderLeft(std::shared_ptr<MenuManager> &instance) {
    if (ImGui::BeginChild("Choices", ImVec2(200, 0), true)) {
        ImGui::Text("Menus");
        ImGui::Separator();
        if (ImGui::Button("Refresh", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_shouldRefresh = true;
        }
        if (ImGui::Button("Add Menu", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = "";
            this->m_descriptionField = "";
            ImGui::OpenPopup("Add Menu");
        }
        if (ImGui::BeginPopupModal("Add Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Add Menu feature");
            ImGui::Separator();
            ImGui::TextUnformatted("Name");
            ImGui::InputText("##Name", &m_nameField);
            ImGui::TextUnformatted("Description");
            ImGui::InputTextMultiline("##Description", &m_descriptionField,
                                      ImVec2(400, ImGui::GetTextLineHeight() * 4));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                instance->SaveMenu(std::make_shared<Menu>(m_nameField, m_descriptionField));
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Edit Menu", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = instance->GetMenuByID(this->m_CurrentChoiceID)->GetName();
            this->m_descriptionField = instance->GetMenuByID(this->m_CurrentChoiceID)->GetDescription();
            ImGui::OpenPopup("Edit Menu");
        }
        if (ImGui::BeginPopupModal("Edit Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto menu = instance->GetMenuByID(this->m_CurrentChoiceID);
            ImGui::Text("Edit Menu: %s", menu->GetName().c_str());
            ImGui::Separator();
            ImGui::TextUnformatted("Name");
            ImGui::InputText("##Name", &m_nameField);
            ImGui::TextUnformatted("Description");
            ImGui::InputTextMultiline("##Description", &m_descriptionField,
                                      ImVec2(400, ImGui::GetTextLineHeight() * 4));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                this->m_shouldRefresh = true;
                menu->SetName(m_nameField);
                menu->SetDescription(m_descriptionField);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Delete Menu", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            ImGui::OpenPopup("Delete Menu");
        }
        if (ImGui::BeginPopupModal("Delete Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete this menu(name: %s)?",
                        instance->GetMenuByID(this->m_CurrentChoiceID)->GetName().c_str());
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                instance->RemoveMenu(this->m_CurrentChoiceID);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Add Section to Menu", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_SelectedID = -1;
            ImGui::OpenPopup("Add Section to Menu");
        }
        if (ImGui::BeginPopupModal("Add Section to Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto menu = instance->GetMenuByID(this->m_CurrentChoiceID);
            auto sections = instance->GetSections();
            ImGui::Text("Add Section to Menu: %s", menu->GetName().c_str());
            ImGui::Separator();
            std::string prev = "Select a section";
            if (m_SelectedID != -1)
                prev = instance->GetSectionByID(m_SelectedID)->GetTitle();
            if (ImGui::BeginCombo("Sections", prev.c_str(), ImGuiComboFlags_None)) {
                static ImGuiTextFilter filter;
                if (ImGui::IsWindowFocused()) {
                    filter.Clear();
                }
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
                filter.Draw("##Filter", -1);
                for (auto &section: sections) {
                    bool is_selected = (m_SelectedID == section->GetID());
                    if (filter.PassFilter(section->GetTitle().c_str())) {
                        if (ImGui::Selectable(section->GetTitle().c_str(), is_selected)) {
                            m_SelectedID = section->GetID();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (m_SelectedID != -1) {
                    const auto section = instance->GetSectionByID(m_SelectedID);
                    if (!menu->ContainsSection(section->GetID())) {
                        menu->AddSection(section);
                        this->m_shouldRefresh = true;
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::EndDisabled();
        }
        ImGui::EndChild();
    }
    return false;
}

void MenuSubMenuScreen::RenderRight(std::shared_ptr<MenuManager> &instance) {
    if (!ImGui::BeginChild("MenuTableChild", ImVec2(0, 0), true)) {
        return;
    }
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 500 - ImGui::CalcTextSize("Filter (Ctrl+F):").x - 20);
    ImGui::TextUnformatted("Filter (Ctrl+F):");
    ImGui::SameLine();
    ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
    m_filter.Draw("##Filter", 500);
    if (ImGui::BeginTable("MenuTable", 4,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                          ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti |
                          ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentDisable, 50.0f);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable, 150.0f);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Sections Count", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableHeadersRow();
        ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs();
        if (m_shouldRefresh || (sortSpecs && sortSpecs->SpecsDirty)) {
            m_CurrentChoiceID = -1;
            m_currentMenus = instance->GetMenus();
            if (sortSpecs) {
                std::ranges::sort(m_currentMenus, [sortSpecs](
                              const std::shared_ptr<Menu> &a,
                              const std::shared_ptr<Menu> &b) {
                                      int column = sortSpecs->Specs->ColumnIndex;
                                      switch (column) {
                                          case 0:
                                              return a->GetID() < b->GetID();
                                          case 1:
                                              return a->GetName() < b->GetName();
                                          case 2:
                                              return a->GetDescription() < b->GetDescription();
                                          case 3:
                                              return a->GetSections().size() < b->GetSections().size();
                                          default:
                                              return false;
                                      }
                                  });
                if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                    std::ranges::reverse(m_currentMenus);
                }
                sortSpecs->SpecsDirty = false;
            }
            m_shouldRefresh = false;
        }
        for (auto &menu: m_currentMenus) {
            if (m_filter.PassFilter(menu->GetName().c_str()) == false && m_filter.PassFilter(
                    menu->GetDescription().c_str())) {
                continue;
            }
            ImGui::TableNextRow();
            ImGui::PushID(menu->GetID());
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", menu->GetID());
            ImGui::TableSetColumnIndex(1);

            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                       ImGuiTreeNodeFlags_SpanAllColumns |
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DrawLinesToNodes |
                                       ImGuiTreeNodeFlags_OpenOnArrow;
            if (m_CurrentChoiceID == menu->GetID()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            bool open = ImGui::TreeNodeEx(menu->GetName().c_str(), flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                if (m_CurrentChoiceID == menu->GetID()) {
                    m_CurrentChoiceID = -1;
                } else {
                    m_CurrentChoiceID = menu->GetID();
                }
            }
            ImGui::TableSetColumnIndex(2);
            ImGui::TextWrapped("%s", menu->GetDescription().c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%d", static_cast<int>(menu->GetSections().size()));
            if (open) {
                for (auto &section: menu->GetSections()) {
                    ImGui::TableNextColumn();
                    ImGui::PushID(section->GetID());
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TreeNodeEx(section->GetTitle().c_str(),
                                      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf |
                                      ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                      ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextWrapped("%s", section->GetDescription().c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::TextUnformatted("");
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

SectionSubMenuScreen::SectionSubMenuScreen() {
    m_ID = "SectionSubMenu";
}

bool SectionSubMenuScreen::RenderLeft(std::shared_ptr<MenuManager> &instance) {
    bool ret = false;
    if (ImGui::BeginChild("Choices", ImVec2(200, 0),
                          true)) {
        ImGui::Text("Sections");
        ImGui::Separator();
        if (ImGui::Button("Refresh", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_shouldRefresh = true;
        }
        if (ImGui::Button("Add Section", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = "";
            this->m_descriptionField = "";
            ImGui::OpenPopup("AddSectionPopup");
        }
        if (ImGui::BeginPopupModal("AddSectionPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Add Section feature.");
            ImGui::Separator();
            ImGui::TextUnformatted("Title");
            ImGui::InputText("##Title", &m_nameField);
            ImGui::TextUnformatted("Description");
            ImGui::InputTextMultiline("##Description", &m_descriptionField,
                                      ImVec2(400, ImGui::GetTextLineHeight() * 4));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                this->m_shouldRefresh = true;
                instance->SaveSection(std::make_shared<MenuSection>(m_nameField, m_descriptionField));
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Edit Section", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = instance->GetSectionByID(this->m_CurrentChoiceID)->GetTitle();
            this->m_descriptionField = instance->GetSectionByID(this->m_CurrentChoiceID)->GetDescription();
            ImGui::OpenPopup("EditSectionPopup");
        }
        if (ImGui::BeginPopupModal("EditSectionPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto section = instance->GetSectionByID(this->m_CurrentChoiceID);
            ImGui::Text("Edit Section: %s", section->GetTitle().c_str());
            ImGui::Separator();
            ImGui::TextUnformatted("Title");
            ImGui::InputText("##Title", &m_nameField);
            ImGui::TextUnformatted("Description");
            ImGui::InputTextMultiline("##Description", &m_descriptionField,
                                      ImVec2(400, ImGui::GetTextLineHeight() * 4));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                this->m_shouldRefresh = true;
                section->SetTitle(m_nameField);
                section->SetDescription(m_descriptionField);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Delete Section", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            ImGui::OpenPopup("DeleteSectionPopup");
        }
        if (ImGui::BeginPopupModal("DeleteSectionPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete this section(name: %s)?",
                        instance->GetSectionByID(this->m_CurrentChoiceID)->GetTitle().c_str());
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                instance->RemoveSection(this->m_CurrentChoiceID);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Add Items", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_SelectedID = -1;
            ImGui::OpenPopup("AddItemsPopup");
        }
        if (ImGui::BeginPopupModal("AddItemsPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto section = instance->GetSectionByID(this->m_CurrentChoiceID);
            auto items = instance->GetMenuItems();
            ImGui::Text("Add Items to Section: %s", section->GetTitle().c_str());
            ImGui::Separator();
            std::string prev = "Select an item";
            if (m_SelectedID != -1)
                prev = instance->GetMenuItemByID(m_SelectedID)->GetTitle();
            if (ImGui::BeginCombo("Items", prev.c_str(), ImGuiComboFlags_None)) {
                static ImGuiTextFilter filter;
                if (ImGui::IsWindowFocused()) {
                    filter.Clear();
                }
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
                filter.Draw("##Filter", -1);
                for (auto &item: items) {
                    bool is_selected = (m_SelectedID == item->GetID());
                    if (filter.PassFilter(item->GetTitle().c_str())) {
                        if (ImGui::Selectable(item->GetTitle().c_str(), is_selected)) {
                            m_SelectedID = item->GetID();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (m_SelectedID != -1) {
                    const auto item = instance->GetMenuItemByID(m_SelectedID);
                    if (!section->ContainsItem(item->GetID())) {
                        section->AddMenuItem(item);
                        this->m_shouldRefresh = true;
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::EndDisabled();
        }
        ImGui::EndChild();
    }
    return ret;
}

void SectionSubMenuScreen::RenderRight(std::shared_ptr<MenuManager> &instance) {
    if (!ImGui::BeginChild("SectionTableChild", ImVec2(0, 0), true)) {
        return;
    }
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 500 - ImGui::CalcTextSize("Filter (Ctrl+F):").x - 20);
    ImGui::TextUnformatted("Filter (Ctrl+F):");
    ImGui::SameLine();
    ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
    m_filter.Draw("##Filter", 500);
    if (ImGui::BeginTable("SectionTable", 4,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                          ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti |
                          ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentDisable, 50.0f);
        ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable, 250.0f);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Items Count", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableHeadersRow();
        ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs();
        if (m_shouldRefresh || (sortSpecs && sortSpecs->SpecsDirty)) {
            m_CurrentChoiceID = -1;
            m_shouldRefresh = false;
            m_currentSections = instance->GetSections();
            if (sortSpecs) {
                std::ranges::sort(m_currentSections, [sortSpecs](
                              const std::shared_ptr<MenuSection> &a,
                              const std::shared_ptr<MenuSection> &b) {
                                      int column = sortSpecs->Specs->ColumnIndex;
                                      switch (column) {
                                          case 0:
                                              return a->GetID() < b->GetID();
                                          case 1:
                                              return a->GetTitle() < b->GetTitle();
                                          case 2:
                                              return a->GetDescription() < b->GetDescription();
                                          case 3:
                                              return a->GetMenuItems().size() < b->GetMenuItems().size();
                                          default:
                                              return false;
                                      }
                                  });
                if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                    std::ranges::reverse(m_currentSections);
                }
                sortSpecs->SpecsDirty = false;
            }
        }
        for (const auto &section: m_currentSections) {
            if (m_filter.PassFilter(section->GetTitle().c_str()) == false &&
                m_filter.PassFilter(section->GetDescription().c_str())
            ) {
                continue;
            }
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", section->GetID());
            ImGui::TableSetColumnIndex(1);
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                       ImGuiTreeNodeFlags_SpanAllColumns |
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DrawLinesToNodes |
                                       ImGuiTreeNodeFlags_OpenOnArrow;
            if (m_CurrentChoiceID == section->GetID()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            bool open = ImGui::TreeNodeEx(section->GetTitle().c_str(), flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                if (m_CurrentChoiceID == section->GetID())
                    m_CurrentChoiceID = -1;
                else
                    m_CurrentChoiceID = section->GetID();
            }
            ImGui::TableSetColumnIndex(2);
            ImGui::TextWrapped("%s", section->GetDescription().c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%d", static_cast<int>(section->GetMenuItems().size()));
            if (open) {
                for (const auto &item: section->GetMenuItems()) {
                    ImGui::TableNextColumn();
                    ImGui::PushID(item->GetID());
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TreeNodeEx(item->GetTitle().c_str(),
                                      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf |
                                      ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                      ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextWrapped("%s", item->GetDescription().c_str());
                    ImGui::TableSetColumnIndex(3);
                    ImGui::TextUnformatted("");
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

ItemSubMenuScreen::ItemSubMenuScreen() {
    m_ID = "ItemSubMenu";
}

bool ItemSubMenuScreen::RenderLeft(std::shared_ptr<MenuManager> &instance) {
    bool ret = false;
    if (ImGui::BeginChild("Choices", ImVec2(200, 0), true)) {
        ImGui::Text("Items");
        ImGui::Separator();
        if (ImGui::Button("Refresh", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_shouldRefresh = true;
        }
        if (ImGui::Button("Add Item", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = "";
            this->m_descriptionField = "";
            this->m_priceField = 0;
            ImGui::OpenPopup("Add Item");
        }
        if (ImGui::BeginPopupModal("Add Item", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Add Item feature.");
            ImGui::Separator();
            ImGui::TextUnformatted("Title");
            ImGui::InputText("##Title", &m_nameField);
            ImGui::TextUnformatted("Description");
            ImGui::InputTextMultiline("##Description", &m_descriptionField,
                                      ImVec2(400, ImGui::GetTextLineHeight() * 4));
            ImGui::TextUnformatted("Price");
            ImGui::InputFloat("VND##Price", &m_priceField);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
                instance->SaveMenuItem(std::make_shared<MenuItem>(m_nameField, m_descriptionField, m_priceField));
                this->m_shouldRefresh = true;
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Edit Item", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = instance->GetMenuItemByID(this->m_CurrentChoiceID)->GetTitle();
            this->m_descriptionField = instance->GetMenuItemByID(this->m_CurrentChoiceID)->GetDescription();
            this->m_priceField = static_cast<float>(instance->GetMenuItemByID(this->m_CurrentChoiceID)->GetPrice());
            ImGui::OpenPopup("EditItemPopup");
        }
        if (ImGui::BeginPopupModal("EditItemPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto item = instance->GetMenuItemByID(this->m_CurrentChoiceID);
            ImGui::Text("Edit Item: %s", item->GetTitle().c_str());
            ImGui::Separator();
            ImGui::TextUnformatted("Title");
            ImGui::InputText("##Title", &m_nameField);
            ImGui::TextUnformatted("Description");
            ImGui::InputTextMultiline("##Description", &m_descriptionField,
                                      ImVec2(400, ImGui::GetTextLineHeight() * 4));
            ImGui::TextUnformatted("Price");
            ImGui::InputFloat("VND##Price", &m_priceField);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                item->SetTitle(m_nameField);
                item->SetDescription(m_descriptionField);
                item->SetPrice(m_priceField);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            ImGui::SetItemDefaultFocus();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Delete Item", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            ImGui::OpenPopup("DeleteItemPopup");
        }
        if (ImGui::BeginPopupModal("DeleteItemPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete this item(name: %s)?",
                        instance->GetMenuItemByID(this->m_CurrentChoiceID)->GetTitle().c_str());
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                instance->RemoveMenuItem(this->m_CurrentChoiceID);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Add Addons", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_SelectedID = -1;
            ImGui::OpenPopup("AddAddonsPopup");
        }
        if (ImGui::BeginPopupModal("AddAddonsPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto item = instance->GetMenuItemByID(this->m_CurrentChoiceID);
            auto addons = instance->GetMenuAddons();
            ImGui::Text("Add Addons to Item: %s", item->GetTitle().c_str());
            ImGui::Separator();
            std::string prev = "Select an addon";
            if (m_SelectedID != -1)
                prev = instance->GetMenuAddonByID(m_SelectedID)->GetName();
            if (ImGui::BeginCombo("Addons", prev.c_str(), ImGuiComboFlags_None)) {
                static ImGuiTextFilter filter;
                if (ImGui::IsWindowFocused()) {
                    filter.Clear();
                }
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
                filter.Draw("##Filter", -1);
                for (auto &addon: addons) {
                    bool is_selected = (m_SelectedID == addon->GetID());
                    if (filter.PassFilter(addon->GetName().c_str())) {
                        if (ImGui::Selectable(addon->GetName().c_str(), is_selected)) {
                            m_SelectedID = addon->GetID();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (m_SelectedID != -1) {
                    const auto addon = instance->GetMenuAddonByID(m_SelectedID);
                    if (!item->ContainsAddon(addon->GetID())) {
                        item->AddAddon(addon);
                        this->m_shouldRefresh = true;
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Remove Addons", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_SelectedID = -1;
            ImGui::OpenPopup("RemoveAddonsPopup");
        }
        if (ImGui::BeginPopupModal("RemoveAddonsPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto item = instance->GetMenuItemByID(this->m_CurrentChoiceID);
            auto addons = item->GetAvailableAddons();
            ImGui::Text("Remove Addons from Item: %s", item->GetTitle().c_str());
            ImGui::Separator();
            std::string prev = "Select an addon";
            if (m_SelectedID != -1)
                prev = item->GetAddon(m_SelectedID)->GetName();
            if (ImGui::BeginCombo("Addons", prev.c_str(), ImGuiComboFlags_None)) {
                static ImGuiTextFilter filter;
                if (ImGui::IsWindowFocused()) {
                    filter.Clear();
                }
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
                filter.Draw("##Filter", -1);
                for (auto &addon: addons) {
                    bool is_selected = (addon->GetID() == m_SelectedID);
                    if (filter.PassFilter(addon->GetName().c_str())) {
                        if (ImGui::Selectable(addon->GetName().c_str(), is_selected)) {
                            m_SelectedID = addon->GetID();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (m_SelectedID != -1) {
                    const auto addon = instance->GetMenuAddonByID(m_SelectedID);
                    if (item->ContainsAddon(addon->GetID())) {
                        item->RemoveAddon(addon);
                        this->m_shouldRefresh = true;
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Edit Ingredient", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_ingredientsField = instance->GetMenuItemByID(this->m_CurrentChoiceID)->GetIngredientsMap();
            this->m_deletedField.clear();
            this->m_SelectedID = -1;
            ImGui::OpenPopup("EditIngredient");
        }
        if (ImGui::BeginPopupModal("EditIngredient", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Edit Ingredient of: %s",
                        instance->GetMenuItemByID(this->m_CurrentChoiceID)->GetTitle().c_str());
            ImGui::Separator();
            ImGui::TextUnformatted("Ingredients (separated by commas)");
            auto ingredients = StorageManager::GetInstance().GetStorages().front()->GetResources();
            std::string prev = "Select ingredients";
            if (m_SelectedID != -1) {
                prev = ingredients[m_SelectedID]->GetName();
            }
            if (ImGui::BeginCombo("##Ingredients", prev.c_str(), ImGuiComboFlags_None)) {
                static ImGuiTextFilter filter;
                if (ImGui::IsWindowFocused()) {
                    filter.Clear();
                }
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
                filter.Draw("##Filter", -1);
                for (auto &ingredient: ingredients) {
                    bool is_selected = m_SelectedID == ingredient->GetID();
                    if (filter.PassFilter(ingredient->GetName().c_str()) && !m_ingredientsField.contains(
                            ingredient->GetName())) {
                        if (ImGui::Selectable(ingredient->GetName().c_str(), is_selected)) {
                            m_SelectedID = ingredient->GetID();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Ingredient")) {
                if (m_SelectedID != -1) {
                    const auto &ingredient = ingredients[m_SelectedID];
                    if (m_ingredientsField.find(ingredient->GetName()) == m_ingredientsField.end()) {
                        m_ingredientsField[ingredient->GetName()] = 1;
                    } else {
                        m_ingredientsField[ingredient->GetName()] += 1;
                    }
                    m_deletedField[ingredient->GetName()] = false;
                    m_SelectedID = -1;
                }
            }
            ImGui::Separator();
            if (ImGui::BeginChild("##IngredientChild", ImVec2(500, 300), true)) {
                if (ImGui::BeginTable("IngredientTable", 3,
                                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                      ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti |
                                      ImGuiTableFlags_ScrollY)) {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn(
                        "Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable,
                        150.0f);
                    ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();
                    for (auto &[name, deleted]: m_deletedField) {
                        if (deleted && m_ingredientsField.contains(name)) {
                            m_ingredientsField.erase(name);
                        }
                    }
                    for (auto &[name, quantity]: m_ingredientsField) {
                        ImGui::TableNextRow();
                        ImGui::PushID(name.c_str());
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", name.c_str());
                        ImGui::TableSetColumnIndex(1);
                        if (quantity < 0) quantity = 0;
                        ImGui::InputInt("##Quantity", &quantity);
                        ImGui::TableSetColumnIndex(2);
                        if (ImGui::Button("Remove")) {
                            m_deletedField[name] = true;
                            m_SelectedID = -1;
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTable();
                }
                ImGui::EndChild();
            }
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                for (auto &[name, quantity]: m_ingredientsField) {
                    if (quantity <= 0) {
                        m_ingredientsField.erase(name);
                    } else {
                        instance->GetMenuItemByID(this->m_CurrentChoiceID)->AddIngredient(name, quantity);
                    }
                }
                for (auto &[name, deleted]: m_deletedField) {
                    if (deleted) {
                        instance->GetMenuItemByID(this->m_CurrentChoiceID)->RemoveIngredient(name);
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::EndDisabled();
        }
    }
    ImGui::EndChild();
    return ret;
}

void ItemSubMenuScreen::RenderRight(std::shared_ptr<MenuManager> &instance) {
    if (!ImGui::BeginChild("ItemTableChild", ImVec2(0, 0), true)) {
        return;
    }
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 500 - ImGui::CalcTextSize("Filter (Ctrl+F):").x - 20);
    ImGui::TextUnformatted("Filter (Ctrl+F):");
    ImGui::SameLine();
    ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
    m_filter.Draw("##Filter", 500);
    if (ImGui::BeginTable("ItemTable", 4,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                          ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti |
                          ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentDisable, 50.0f);
        ImGui::TableSetupColumn("Title", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable, 150.0f);
        ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Price", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableHeadersRow();
        ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs();
        if (m_shouldRefresh || (sortSpecs && sortSpecs->SpecsDirty)) {
            m_CurrentChoiceID = -1;
            m_shouldRefresh = false;
            m_currentItems = instance->GetMenuItems();
            if (sortSpecs) {
                std::ranges::sort(m_currentItems, [sortSpecs](
                              const std::shared_ptr<MenuItem> &a,
                              const std::shared_ptr<MenuItem> &b) {
                                      int column = sortSpecs->Specs->ColumnIndex;
                                      switch (column) {
                                          case 0:
                                              return a->GetID() < b->GetID();
                                          case 1:
                                              return a->GetTitle() < b->GetTitle();
                                          case 2:
                                              return a->GetDescription() < b->GetDescription();
                                          case 3:
                                              return a->GetPrice() < b->GetPrice();
                                          default:
                                              return false;
                                      }
                                  });
                if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                    std::ranges::reverse(m_currentItems);
                }
                sortSpecs->SpecsDirty = false;
            }
        }
        for (const auto &item: m_currentItems) {
            if (m_filter.PassFilter(item->GetTitle().c_str()) == false && m_filter.PassFilter(
                    item->GetDescription().c_str()) == false) {
                continue;
            }
            ImGui::TableNextRow();
            ImGui::PushID(item->GetID());
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", item->GetID());
            ImGui::TableSetColumnIndex(1);
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                       ImGuiTreeNodeFlags_SpanAllColumns |
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DrawLinesToNodes |
                                       ImGuiTreeNodeFlags_OpenOnArrow;
            if (m_CurrentChoiceID == item->GetID()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            bool open = ImGui::TreeNodeEx(item->GetTitle().c_str(), flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                if (m_CurrentChoiceID == item->GetID()) {
                    m_CurrentChoiceID = -1;
                } else {
                    m_CurrentChoiceID = item->GetID();
                }
            }
            ImGui::TableSetColumnIndex(2);
            ImGui::TextWrapped("%s", item->GetDescription().c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text("%s", CurrencyUtils::FormatUSD(item->GetPrice()).c_str());
            if (open) {
                for (const auto &addon: item->GetAvailableAddons()) {
                    ImGui::TableNextColumn();
                    ImGui::PushID(addon->GetID());
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TreeNodeEx(addon->GetName().c_str(),
                                      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf |
                                      ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                      ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextUnformatted("");
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%s", CurrencyUtils::FormatUSD(addon->GetPrice()).c_str());
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}

AddonSubMenuScreen::AddonSubMenuScreen() {
    m_ID = "AddonSubMenu";
}

bool AddonSubMenuScreen::RenderLeft(std::shared_ptr<MenuManager> &instance) {
    if (ImGui::BeginChild("Choices", ImVec2(200, 0), true)) {
        ImGui::Text("Addons");
        ImGui::Separator();
        if (ImGui::Button("Refresh", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_shouldRefresh = true;
        }
        if (ImGui::Button("Add Addon", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = "";
            this->m_priceField = 0;
            ImGui::OpenPopup("Add Addon");
        }
        if (ImGui::BeginPopupModal("Add Addon", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Add Addon feature.");
            ImGui::Separator();
            ImGui::TextUnformatted("Title");
            ImGui::InputText("##Title", &m_nameField);
            ImGui::TextUnformatted("Price");
            ImGui::InputFloat("VND##Price", &m_priceField);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                this->m_shouldRefresh = true;
                instance->SaveMenuAddon(
                    std::make_shared<MenuAddon>(m_nameField, m_priceField, std::vector<MealIngredient>{}));
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Edit Addon", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = instance->GetMenuAddonByID(this->m_CurrentChoiceID)->GetName();
            this->m_priceField = static_cast<float>(instance->GetMenuAddonByID(this->m_CurrentChoiceID)->GetPrice());
            ImGui::OpenPopup("Edit Addon");
        }
        if (ImGui::BeginPopupModal("Edit Addon", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto addon = instance->GetMenuAddonByID(this->m_CurrentChoiceID);
            ImGui::Text("Edit Addon: %s",
                        addon->GetName().c_str());
            ImGui::Separator();
            ImGui::TextUnformatted("Title");
            ImGui::InputText("##Title", &m_nameField);
            ImGui::TextUnformatted("Price");
            ImGui::InputFloat("VND##Price", &m_priceField);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                addon->SetName(m_nameField);
                addon->SetPrice(m_priceField);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Delete Addon", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            ImGui::OpenPopup("Delete Addon");
        }
        if (ImGui::BeginPopupModal("Delete Addon", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete this addon(name: %s)?",
                        instance->GetMenuAddonByID(this->m_CurrentChoiceID)->GetName().c_str());
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                instance->RemoveMenuAddon(this->m_CurrentChoiceID);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Edit Ingredients", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_deletedField.clear();
            this->m_ingredientsField = instance->GetMenuAddonByID(this->m_CurrentChoiceID)->GetIngredientsMap();
            this->m_SelectedID = -1;
            ImGui::OpenPopup("EditIngredients");
        }
        if (ImGui::BeginPopupModal("EditIngredients", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Edit Ingredient of: %s",
                        instance->GetMenuAddonByID(this->m_CurrentChoiceID)->GetName().c_str());
            ImGui::Separator();
            ImGui::TextUnformatted("Ingredients (separated by commas)");
            auto ingredients = StorageManager::GetInstance().GetStorages().front()->GetResources();
            std::string prev = "Select ingredients";
            if (m_SelectedID != -1) {
                prev = ingredients[m_SelectedID]->GetName();
            }
            if (ImGui::BeginCombo("##Ingredients", prev.c_str(), ImGuiComboFlags_None)) {
                static ImGuiTextFilter filter;
                if (ImGui::IsWindowFocused()) {
                    filter.Clear();
                }
                ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
                filter.Draw("##Filter", -1);
                for (auto &ingredient: ingredients) {
                    bool is_selected = m_SelectedID == ingredient->GetID();
                    if (filter.PassFilter(ingredient->GetName().c_str()) && !m_ingredientsField.contains(
                            ingredient->GetName())) {
                        if (ImGui::Selectable(ingredient->GetName().c_str(), is_selected)) {
                            m_SelectedID = ingredient->GetID();
                        }
                    }
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            if (ImGui::Button("Add Ingredient")) {
                if (m_SelectedID != -1) {
                    const auto &ingredient = ingredients[m_SelectedID];
                    if (m_ingredientsField.find(ingredient->GetName()) == m_ingredientsField.end()) {
                        m_ingredientsField[ingredient->GetName()] = 1;
                    } else {
                        m_ingredientsField[ingredient->GetName()] += 1;
                    }
                    m_deletedField[ingredient->GetName()] = false;
                    m_SelectedID = -1;
                }
            }
            ImGui::Separator();
            if (ImGui::BeginChild("##IngredientChild", ImVec2(500, 300), true)) {
                if (ImGui::BeginTable("IngredientTable", 3,
                                      ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                                      ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti |
                                      ImGuiTableFlags_ScrollY)) {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn(
                        "Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable,
                        150.0f);
                    ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 200.0f);
                    ImGui::TableSetupColumn("Actions", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();
                    for (auto &[name, deleted]: m_deletedField) {
                        if (deleted && m_ingredientsField.contains(name)) {
                            m_ingredientsField.erase(name);
                        }
                    }
                    for (auto &[name, quantity]: m_ingredientsField) {
                        ImGui::TableNextRow();
                        ImGui::PushID(name.c_str());
                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%s", name.c_str());
                        ImGui::TableSetColumnIndex(1);
                        if (quantity < 0) quantity = 0;
                        ImGui::InputInt("##Quantity", &quantity);
                        ImGui::TableSetColumnIndex(2);
                        if (ImGui::Button("Remove")) {
                            m_deletedField[name] = true;
                            m_SelectedID = -1;
                        }
                        ImGui::PopID();
                    }
                    ImGui::EndTable();
                }
                ImGui::EndChild();
            }
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                for (auto &[name, quantity]: m_ingredientsField) {
                    if (quantity < 0) {
                        m_ingredientsField.erase(name);
                    } else {
                        instance->GetMenuAddonByID(this->m_CurrentChoiceID)->AddIngredient({name, quantity});
                    }
                }
                for (auto &[name, deleted]: m_deletedField) {
                    if (deleted) {
                        instance->GetMenuAddonByID(this->m_CurrentChoiceID)->RemoveIngredient(name);
                    }
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::EndDisabled();
        }
        ImGui::EndChild();
    }
    return false;
}

void AddonSubMenuScreen::RenderRight(std::shared_ptr<MenuManager> &instance) {
    if (!ImGui::BeginChild("AddonTableChild", ImVec2(0, 0), true)) {
        return;
    }
    ImGui::SetCursorPosX(ImGui::GetContentRegionAvail().x - 500 - ImGui::CalcTextSize("Filter (Ctrl+F):").x - 20);
    ImGui::TextUnformatted("Filter (Ctrl+F):");
    ImGui::SameLine();
    ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
    m_filter.Draw("##Filter", 500);
    if (ImGui::BeginTable("AddonTable", 3,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                          ImGuiTableFlags_Sortable | ImGuiTableFlags_SortMulti |
                          ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentDisable, 50.0f);
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable, 200.0f);
        ImGui::TableSetupColumn("Price/Quantity", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableHeadersRow();
        ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs();
        if (m_shouldRefresh || (sortSpecs && sortSpecs->SpecsDirty)) {
            m_CurrentChoiceID = -1;
            m_shouldRefresh = false;
            m_currentAddons = instance->GetMenuAddons();
            if (sortSpecs) {
                std::ranges::sort(m_currentAddons, [sortSpecs](
                              const std::shared_ptr<MenuAddon> &a,
                              const std::shared_ptr<MenuAddon> &b) {
                                      int column = sortSpecs->Specs->ColumnIndex;
                                      switch (column) {
                                          case 0:
                                              return a->GetID() < b->GetID();
                                          case 1:
                                              return a->GetName() < b->GetName();
                                          case 2:
                                              return a->GetPrice() < b->GetPrice();
                                          default:
                                              return false;
                                      }
                                  });
                if (sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                    std::ranges::reverse(m_currentAddons);
                }
                sortSpecs->SpecsDirty = false;
            }
        }
        for (const auto &addon: m_currentAddons) {
            if (m_filter.PassFilter(addon->GetName().c_str()) == false) {
                continue;
            }
            ImGui::TableNextRow();
            ImGui::PushID(addon->GetID());
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", addon->GetID());
            ImGui::TableSetColumnIndex(1);
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                       ImGuiTreeNodeFlags_SpanAllColumns |
                                       ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DrawLinesToNodes |
                                       ImGuiTreeNodeFlags_OpenOnArrow;
            if (m_CurrentChoiceID == addon->GetID()) {
                flags |= ImGuiTreeNodeFlags_Selected;
            }
            bool open = ImGui::TreeNodeEx(addon->GetName().c_str(), flags);
            if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
                if (m_CurrentChoiceID == addon->GetID())
                    m_CurrentChoiceID = -1;
                else
                    m_CurrentChoiceID = addon->GetID();
            }
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", CurrencyUtils::FormatUSD(addon->GetPrice()).c_str());
            if (open) {
                for (const auto &ingr: addon->GetIngredients()) {
                    ImGui::TableNextColumn();
                    ImGui::PushID(ingr.Name.c_str());
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("");
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TreeNodeEx(ingr.Name.c_str(),
                                      ImGuiTreeNodeFlags_SpanFullWidth | ImGuiTreeNodeFlags_Leaf |
                                      ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_LabelSpanAllColumns |
                                      ImGuiTreeNodeFlags_Bullet | ImGuiTreeNodeFlags_NoTreePushOnOpen);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("%d", ingr.Quantity);
                    ImGui::PopID();
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
    ImGui::EndChild();
}
