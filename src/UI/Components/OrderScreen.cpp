#include "OrderScreen.hpp"
#include "imgui.h"
#include "UI/UICore/Core.hpp"
#include <array>

#include "FileMenuRepository.hpp"
#include "FileTableRepository.hpp"
#include "Application/Table/TableManager.hpp"

#include "misc/cpp/imgui_stdlib.h"

OrderScreen::OrderScreen(Core &core, std::shared_ptr<OrderManager> manager,
                         std::shared_ptr<const IMenuRepository> menuRepo,
                         std::shared_ptr<IMealRepository> mealRepo): Screen(core), m_Manager(manager),
                                                                     m_MenuRepo(menuRepo),
                                                                     m_MealRepo(mealRepo) {
}

void OrderScreen::Init() {
    // Optional: load UI state if needed
    auto tableRepo = std::make_shared<FileTableRepository>("Data/Table.json");
    m_TableManager = std::make_shared<TableManager>(tableRepo);
}

void OrderScreen::OnExit() {
    // Optional cleanup
    m_TableManager.reset();
}

void OrderScreen::Render(float dt) {
    ImGui::Text("Restaurant Order Management");
    ImGui::Separator();

    DrawBackButton();
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    m_AvailableTables = m_TableManager->GetTables();
    DrawNewOrderButton();
    ImGui::Spacing();

    DrawOrderTable();
    ImGui::Spacing();

    DrawSaveButton();
    ImGui::SameLine();

    if (m_ShowMealDetailsForOrder != -1) {
        DrawMealDetailsPopup();
    }
}

// Draw table showing all orders and allow edit/delete
void OrderScreen::DrawOrderTable() {
    const auto orders = m_Manager->GetAllOrders();

    if (ImGui::BeginTable("Orders", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Order ID", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Table ID", ImGuiTableColumnFlags_WidthFixed, 120.0f);
        ImGui::TableSetupColumn("Customer Phone", ImGuiTableColumnFlags_WidthFixed,
                                ImGui::CalcTextSize("Customer Phone").x + 80);
        ImGui::TableSetupColumn("Time", ImGuiTableColumnFlags_WidthFixed, 210.0f);
        ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("Total Price", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (const auto &order: orders) {
            int orderID = order->GetID();
            ImGui::TableNextRow();

            // --- Order ID (non-editable) ---
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", orderID);

            // --- Table ID ---
            ImGui::TableSetColumnIndex(1);
            int &tableID = m_EditedTableID[orderID];
            if (tableID == 0) tableID = order->GetTableID();
            ImGui::SetNextItemWidth(100);
            if (ImGui::BeginCombo(("##table" + std::to_string(orderID)).c_str(),
                                  tableID == -1 ? "Select Table" : std::format("Table {}", tableID).c_str())) {
                for (const auto &table: m_AvailableTables) {
                    std::string tableLabel = "Table " + std::to_string(table->GetTableID()) + " (Seats: " +
                                             std::to_string(table->GetMaxCapacity()) + ")";
                    bool isSelected = (tableID == table->GetTableID());
                    if (ImGui::Selectable(tableLabel.c_str(), isSelected)) {
                        tableID = table->GetTableID();
                    }
                    if (isSelected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }

            // --- Customer Phone ---
            ImGui::TableSetColumnIndex(2);
            std::string &customerPhone = m_EditedCustomerPhone[orderID];
            if (customerPhone.empty()) customerPhone = order->GetCustomerPhone();

            ImGui::SetNextItemWidth(-1);
            ImGui::InputText(("##phone" + std::to_string(orderID)).c_str(), &customerPhone);

            // --- Time (Hours and Minutes only) ---
            ImGui::TableSetColumnIndex(3);

            if (m_EditedDateTime.find(orderID) == m_EditedDateTime.end()) {
                m_EditedDateTime[orderID] = order->GetDate();
            }
            DateTime &dt = m_EditedDateTime[orderID];

            ImGui::SameLine();
            std::string buttonText = dt.ToStringTime();
            if (ImGui::Button((buttonText + "##date" + std::to_string(orderID)).c_str())) {
                m_DatePicker.SetDateTime(dt);
                m_DatePicker.SetShowSeconds(true);
                ImGui::OpenPopup(("DatePicker" + std::to_string(orderID)).c_str());
            }
            if (m_DatePicker.RenderTimeOnly(("DatePicker" + std::to_string(orderID)).c_str())) {
                dt = m_DatePicker.GetDateTime();
            }

            // --- Status combo ---
            ImGui::TableSetColumnIndex(4);
            OrderStatus &status = m_EditedStatus[orderID];
            if (status == OrderStatus::None && order->GetStatus() != OrderStatus::None)
                status = order->GetStatus();

            static const std::array<const char *, 5> statusLabels = {
                "None", "Received", "Preparing", "Complete", "Canceled"
            };

            int statusIndex = static_cast<int>(status);
            if (ImGui::Combo(("##status" + std::to_string(orderID)).c_str(), &statusIndex,
                             statusLabels.data(), static_cast<int>(statusLabels.size()))) {
                status = static_cast<OrderStatus>(statusIndex);
            }

            // --- Total Price (read-only) ---
            ImGui::TableSetColumnIndex(5);
            ImGui::Text("$%.2f", order->GetTotalPrice());

            // --- Actions ---
            ImGui::TableSetColumnIndex(6);
            if (ImGui::Button(("Update##" + std::to_string(orderID)).c_str())) {
                order->SetOrderStatus(status);
                order->SetTableID(tableID);
                order->SetDate(dt);
                order->SetCustomerPhone(customerPhone);
            }
            ImGui::SameLine();
            if (ImGui::Button(("View Meals##" + std::to_string(orderID)).c_str())) {
                m_ShowMealDetailsForOrder = orderID;
            }
            ImGui::SameLine();
            if (ImGui::Button(("Edit Meals##" + std::to_string(orderID)).c_str())) {
                m_EditingOrderID = orderID;
                m_ShowMealSelectionPopup = true;
                m_SelectedMenuID = -1;
                m_SelectedMenuSectionID = -1;
                m_SelectedMenuItemQuantities.clear();
                m_OrderID = orderID;
                m_AddonsMap.clear();
                auto item = order->GetMeal();
                for (const auto &mItem: item->GetMealItems()) {
                    m_SelectedMenuItemQuantities[mItem->GetMenuItemID()] = mItem->GetQuantity();
                    for (const auto &addon: mItem->GetAddons()) {
                        m_AddonsMap[mItem->GetMenuItemID()][addon->GetAddon()->GetID()] = addon->GetQuantity();
                    }
                }
                ImGui::OpenPopup("Select Meals");
            }
            if (m_OrderID == orderID) {
                DrawMealSelectionPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button(("Delete##" + std::to_string(orderID)).c_str())) {
                m_Manager->RemoveOrder(orderID);
            }
        }

        ImGui::EndTable();
    }
}

void OrderScreen::DrawMealSelectionPopup() {
    // Handle the modal popup
    if (!ImGui::BeginPopupModal("Select Meals", &m_ShowMealSelectionPopup, ImGuiWindowFlags_AlwaysAutoResize))
        return;
    ImGui::TextUnformatted("Choose Menu: ");
    std::string previewText = "Select Menu";
    if (m_SelectedMenuID != -1) {
        previewText = m_MenuRepo->GetMenuByID(m_SelectedMenuID)->GetName();
    }
    if (ImGui::BeginCombo("##menuSelect", previewText.c_str())) {
        auto menus = m_MenuRepo->GetMenus();
        for (const auto &menu: menus) {
            bool isSelected = (m_SelectedMenuID == menu->GetID());
            if (ImGui::Selectable(menu->GetName().c_str(), isSelected)) {
                m_SelectedMenuID = menu->GetID();
                m_SelectedMenuSectionID = -1;
            }
            if (isSelected) ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (m_SelectedMenuID != -1) {
        std::string sectionPreviewText = "Select Section";
        if (m_SelectedMenuSectionID != -1) {
            sectionPreviewText = m_MenuRepo->GetMenuByID(m_SelectedMenuID)->GetSectionByID(
                m_SelectedMenuSectionID)->GetTitle();
        }
        ImGui::TextUnformatted("Select Menu Section: ");
        if (ImGui::BeginCombo("##menuSectionSelect", sectionPreviewText.c_str())) {
            auto sections = m_MenuRepo->GetMenuByID(m_SelectedMenuID)->GetSections();
            for (const auto &section: sections) {
                bool isSelected = (m_SelectedMenuSectionID == section->GetID());
                if (ImGui::Selectable(section->GetTitle().c_str(), isSelected)) {
                    m_SelectedMenuSectionID = section->GetID();
                    m_SelectedMenuItemQuantities.clear();
                }
            }
            ImGui::EndCombo();
        }
    }
    if (m_SelectedMenuSectionID != -1) {
        if (ImGui::BeginTable("MenuItemSelection", 3,
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_ScrollY,
                              ImVec2(600, 200))) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Selected", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("Meal Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            auto section = m_MenuRepo->GetMenuByID(m_SelectedMenuID)->GetSectionByID(m_SelectedMenuSectionID);
            if (section) {
                auto meals = section->GetMenuItems();
                for (const auto &meal: meals) {
                    int mealID = meal->GetID();

                    ImGui::TableNextRow();

                    // Selection checkbox
                    ImGui::TableSetColumnIndex(0);
                    bool isSelected = (m_SelectedMenuItemQuantities.contains(mealID));
                    if (ImGui::Checkbox(("##selectMeal" + std::to_string(mealID)).c_str(), &isSelected)) {
                        if (!isSelected) {
                            m_SelectedMenuItemQuantities.erase(mealID);
                        } else if (m_SelectedMenuItemQuantities[mealID] == 0) {
                            m_SelectedMenuItemQuantities[mealID] = 1; // Default to 1 if selecting
                        }
                    }

                    // Meal name
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextUnformatted(meal->GetTitle().c_str());
                    // Quantity input
                    ImGui::TableSetColumnIndex(2);
                    if (isSelected) {
                        ImGui::SetNextItemWidth(-1);
                        ImGui::InputInt(("##quantity" + std::to_string(mealID)).c_str(),
                                        &m_SelectedMenuItemQuantities[mealID]);
                        if (m_SelectedMenuItemQuantities[mealID] < 1) m_SelectedMenuItemQuantities[mealID] = 1;
                        // Minimum quantity of 1
                    } else {
                        ImGui::TextUnformatted("-");
                    }
                }
            }

            ImGui::EndTable();
        }
    }
    ImGui::Separator();
    if (ImGui::BeginTable("SelectedItem", 3,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY,
                          ImVec2(600, 200))) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Meal Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 150.0f);
        ImGui::TableSetupColumn("Option", ImGuiTableColumnFlags_WidthFixed, 160.0f);
        ImGui::TableHeadersRow();

        for (auto &[mealID, quantity]: m_SelectedMenuItemQuantities) {
            auto meal = m_MenuRepo->GetMenuItemByID(mealID);
            if (meal) {
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::TextUnformatted(meal->GetTitle().c_str());
                ImGui::TableSetColumnIndex(1);
                ImGui::SetNextItemWidth(-1);
                ImGui::InputInt(std::format("##{}", meal->GetID()).c_str(), &quantity);
                ImGui::TableSetColumnIndex(2);
                if (ImGui::Button(std::format("Remove##{}", meal->GetID()).c_str())) {
                    m_SelectedMenuItemQuantities.erase(mealID);
                }
                ImGui::SameLine();
                if (ImGui::Button(std::format("Addons##{}", meal->GetID()).c_str())) {
                    m_AddonEditPopup = true;
                    ImGui::OpenPopup("Edit Addons");
                    m_MealID = meal->GetID();
                }
                if (meal->GetID() == m_MealID && m_AddonEditPopup)
                    DrawEditAddonPopup(meal->GetID());
            }
        }

        ImGui::EndTable();

        if (ImGui::Button("Done", ImVec2(120, 0))) {
            if (m_OrderID != -1) {
                auto order = m_Manager->GetOrder(m_OrderID);
                if (order) {
                    auto meal = order->GetMeal();
                    for (const auto &id: meal->GetMealItems()) {
                        meal->RemoveItem(id->GetID());
                    }
                    for (auto &[mealID, quantity]: m_SelectedMenuItemQuantities) {
                        auto menuItem = m_MenuRepo->GetMenuItemByID(mealID);
                        if (menuItem) {
                            const auto mealItem = std::make_shared<MealItem>(menuItem, quantity);
                            if (m_AddonsMap.contains(mealID)) {
                                for (auto &[addonID, addonQty]: m_AddonsMap[mealID]) {
                                    auto addon = m_MenuRepo->GetMenuAddonByID(addonID);
                                    if (addon) {
                                        mealItem->AddAddon(addon, addonQty);
                                    }
                                }
                            }
                            meal->AddItem(mealItem);
                        }
                    }
                }
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            m_SelectedMenuItemQuantities.clear();
            ImGui::CloseCurrentPopup();
        }
    }
    ImGui::EndPopup();
}

void OrderScreen::DrawMealDetailsPopup() {
    if (m_ShowMealDetailsForOrder != -1) {
        std::string popupName = "Order Meals Details##" + std::to_string(m_ShowMealDetailsForOrder);
        ImGui::OpenPopup(popupName.c_str());
    }

    std::string popupName = "Order Meals Details##" + std::to_string(m_ShowMealDetailsForOrder);
    bool shouldClose = false;

    if (ImGui::BeginPopupModal(popupName.c_str(), nullptr,
                               (ImGuiWindowFlags_AlwaysAutoResize))) {
        auto orders = m_Manager->GetAllOrders();
        std::shared_ptr<Order> targetOrder = nullptr;

        for (const auto &order: orders) {
            if (order->GetID() == m_ShowMealDetailsForOrder) {
                targetOrder = order;
                break;
            }
        }

        if (targetOrder) {
            ImGui::Text("Order ID: %d", targetOrder->GetID());
            ImGui::Text("Table: %d", targetOrder->GetTableID());
            ImGui::Text("Customer: %s", targetOrder->GetCustomerPhone().c_str());
            ImGui::Text("Total: $%.2f", targetOrder->GetTotalPrice());
            ImGui::Separator();

            auto meal = targetOrder->GetMeal();
            if (ImGui::BeginTable("MealDetails", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Meal ID");
                ImGui::TableSetupColumn("Item Name");
                ImGui::TableSetupColumn("Quantity");
                ImGui::TableSetupColumn("Addons");
                ImGui::TableSetupColumn("Price");
                ImGui::TableHeadersRow();

                auto mealItems = meal->GetMealItems();

                if (mealItems.empty()) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("%d", meal->GetID());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("No items");
                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("-");
                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("-");
                    ImGui::TableSetColumnIndex(4);
                    ImGui::Text("$%.2f", meal->GetPrice());
                } else {
                    for (const auto &item: mealItems) {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", meal->GetID());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("%s", item->GetMenuItemTitle().c_str());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%d", item->GetQuantity());

                        ImGui::TableSetColumnIndex(3);
                        auto addons = item->GetAddons();
                        if (addons.empty()) {
                            ImGui::Text("None");
                        } else {
                            std::string addonStr;
                            for (size_t i = 0; i < addons.size(); ++i) {
                                if (i > 0) addonStr += ", ";
                                addonStr += addons[i]->GetAddon()->GetName();
                                addonStr += " x" + std::to_string(addons[i]->GetQuantity());
                            }
                            ImGui::Text("%s", addonStr.c_str());
                        }

                        ImGui::TableSetColumnIndex(4);
                        ImGui::Text("$%.2f", item->GetPrice());
                    }
                }
                ImGui::EndTable();
            }
        } else {
            ImGui::Text("Order not found!");
        }

        ImGui::Separator();
        if (ImGui::Button("Close")) {
            shouldClose = true;
        }

        ImGui::EndPopup();
    }

    if (shouldClose || !ImGui::IsPopupOpen(popupName.c_str())) {
        m_ShowMealDetailsForOrder = -1;
    }
}

void OrderScreen::DrawNewOrderButton() {
    if (ImGui::Button("New Order")) {
        // Reset input fields
        m_NewTableID = -1;
        m_NewCustomerPhoneStr.clear();
        m_SelectedMealIDs.clear();
        m_SelectedMenuItemQuantities.clear();
        m_NewDate = DateTime::Now();
        this->m_DatePicker.SetDateTime(m_NewDate);
        this->m_DatePicker.SetShowTime(true);
        this->m_DatePicker.SetShowSeconds(true);
        m_AddonsMap.clear();
        m_confirm = false;
        m_OrderID = -1;

        // Open the popup
        ImGui::OpenPopup("Create New Order");
    }

    // Handle the modal popup
    if (ImGui::BeginPopupModal("Create New Order", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Create a New Order");
        ImGui::Separator();
        // Table ID input
        if (m_confirm && m_NewTableID == -1)
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Table ID is required!");
        if (ImGui::BeginCombo("Table ID", m_NewTableID == -1
                                              ? "Select Table"
                                              : std::format("Table {}", m_NewTableID).c_str())) {
            for (const auto &table: m_AvailableTables) {
                std::string tableLabel = "Table " + std::to_string(table->GetTableID()) + " (Seats: " +
                                         std::to_string(table->GetMaxCapacity()) + ")";
                bool isSelected = (m_NewTableID == table->GetTableID());
                if (ImGui::Selectable(tableLabel.c_str(), isSelected)) {
                    m_NewTableID = table->GetTableID();
                }
                if (isSelected) ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Customer Phone input
        if (m_confirm && m_NewCustomerPhoneStr.empty())
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "Customer phone is required!");
        else
            ImGui::TextUnformatted("Customer Phone");
        ImGui::InputText("##CustomerPhone", &m_NewCustomerPhoneStr);

        ImGui::Spacing();

        // Select Meals button
        if (m_confirm && m_SelectedMenuItemQuantities.empty())
            ImGui::TextColored(ImVec4(1, 0, 0, 1), "At least one meal must be selected!");
        if (ImGui::Button("Select Meals", ImVec2(200, 0))) {
            m_SelectedMenuID = -1;
            m_SelectedMenuSectionID = -1;
            m_SelectedMenuItemQuantities.clear();
            m_AddonsMap.clear();
            m_ShowMealSelectionPopup = true;
            m_confirm = false;
            m_OrderID = -1;
            ImGui::OpenPopup("Select Meals");
        }
        DrawMealSelectionPopup();

        ImGui::SameLine();
        // Show selected meals summary
        if (m_SelectedMenuItemQuantities.empty()) {
            ImGui::Text("Selected: 0 meals");
        } else {
            int totalMeals = 0;
            for (const auto &[mealID, quantity]: m_SelectedMenuItemQuantities) {
                totalMeals += quantity;
            }
            ImGui::Text("Selected: %d meals (%zu unique)", totalMeals, m_SelectedMenuItemQuantities.size());
        }

        ImGui::TextUnformatted("Reservation Time: ");
        ImGui::SameLine();
        std::string buttonText = m_NewDate.ToStringDateTime();
        if (ImGui::Button(buttonText.c_str())) {
            ImGui::OpenPopup("DatePicker");
        }
        if (m_DatePicker.Render("DatePicker")) {
            m_NewDate = m_DatePicker.GetDateTime();
        }
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Spacing();

        // OK and Cancel buttons
        if (ImGui::Button("Create Order", ImVec2(120, 0))) {
            m_confirm = true;
            if (m_NewTableID != -1 && !m_NewCustomerPhoneStr.empty() && !m_SelectedMenuItemQuantities.empty()) {
                auto meal = std::make_shared<Meal>(m_NewDate);

                if (meal) {
                    for (const auto &[mealID, quantity]: m_SelectedMenuItemQuantities) {
                        auto item = meal->AddItem(m_MenuRepo->GetMenuItemByID(mealID), quantity);
                        if (m_AddonsMap.contains(mealID)) {
                            for (const auto &[addonID, addonQty]: m_AddonsMap[mealID]) {
                                auto addon = m_MenuRepo->GetMenuAddonByID(addonID);
                                item->AddAddon(addon, addonQty);
                            }
                        }
                    }
                    m_Manager->CreateOrder(m_NewTableID, m_NewCustomerPhoneStr, m_NewDate, meal);

                    // Clear inputs and close popup
                    m_NewTableID = -1;
                    m_NewCustomerPhoneStr.clear();
                    m_SelectedMealIDs.clear();
                    m_SelectedMenuItemQuantities.clear();
                    ImGui::CloseCurrentPopup();
                } else
                    LOG_WARNING("Failed to create meal for new order.");
            }
            else
                LOG_WARNING("Failed to create order. Missing required fields.");
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            // Clear inputs and close popup
            m_NewTableID = -1;;
            m_NewCustomerPhoneStr.clear();
            m_SelectedMealIDs.clear();
            m_SelectedMenuItemQuantities.clear();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}


void OrderScreen::DrawEditAddonPopup(int menuID) {
    if (!ImGui::BeginPopupModal("Edit Addons", &m_AddonEditPopup, ImGuiWindowFlags_AlwaysAutoResize))
        return;
    auto menuItemm = m_MenuRepo->GetMenuItemByID(menuID);
    if (!menuItemm) {
        ImGui::Text("Menu item not found!");
    } else {
        auto &m_AddonMap = m_AddonsMap[menuID];
        if (ImGui::BeginTable("EditAddonsTable", 3,
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_ScrollY,
                              ImVec2(400, 300))) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("Selected", ImGuiTableColumnFlags_WidthFixed, 60.0f);
            ImGui::TableSetupColumn("Addon Name", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Quantity", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            auto addons = menuItemm->GetAvailableAddons();
            for (const auto &addon: addons) {
                int addonID = addon->GetID();

                ImGui::TableNextRow();

                // Selection checkbox
                ImGui::TableSetColumnIndex(0);
                bool isSelected = (m_AddonMap.contains(addonID));
                if (ImGui::Checkbox(("##selectAddon" + std::to_string(addonID)).c_str(), &isSelected)) {
                    if (!isSelected) {
                        m_AddonMap.erase(addonID);
                    } else {
                        m_AddonMap[addonID] = 1; // Default to 1 if selecting
                    }
                }

                // Addon name
                ImGui::TableSetColumnIndex(1);
                ImGui::TextUnformatted(addon->GetName().c_str());
                // Quantity input
                ImGui::TableSetColumnIndex(2);
                if (isSelected) {
                    ImGui::SetNextItemWidth(-1);
                    ImGui::InputInt(("##quantity" + std::to_string(addonID)).c_str(),
                                    &m_AddonMap[addonID]);
                    if (m_AddonMap[addonID] < 1) m_AddonMap[addonID] = 1;
                    // Minimum quantity of 1
                } else {
                    ImGui::TextUnformatted("-");
                }
            }
            ImGui::EndTable();
        }
        if (ImGui::Button("Save")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
    }
    if (ImGui::Button("Close")) {
        ImGui::CloseCurrentPopup();
    }
    ImGui::EndPopup();
}


void OrderScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        m_Manager->SaveAllOrders();
    }
}

void OrderScreen::DrawBackButton() {
    if (ImGui::Button("Go Back")) {
        m_Core.PopScreen();
    }
}

const char *OrderScreen::StatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::None: return "None";
        case OrderStatus::Received: return "Received";
        case OrderStatus::Preparing: return "Preparing";
        case OrderStatus::Complete: return "Complete";
        case OrderStatus::Canceled: return "Canceled";
        default: return "Unknown";
    }
}

std::vector<OrderStatus> OrderScreen::GetValidOrderStatuses() {
    return {
        OrderStatus::None,
        OrderStatus::Received,
        OrderStatus::Preparing,
        OrderStatus::Complete,
        OrderStatus::Canceled
    };
}
