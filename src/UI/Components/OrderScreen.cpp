#include "OrderScreen.hpp"
#include "imgui.h"
#include "UI/UICore/Core.hpp"
#include <array>
#include <algorithm>

#include "FileMenuRepository.hpp"

OrderScreen::OrderScreen(Core &core, std::shared_ptr<OrderManager> manager, std::shared_ptr<IMenuRepository> menuRepo,
                         std::shared_ptr<IMealRepository> mealRepo): Screen(core), m_Manager(manager),
                                                                     m_MenuRepo(menuRepo),
                                                                     m_MealRepo(mealRepo) {
    m_NewTableIDInput.resize(256);
    m_NewCustomerPhoneInput.resize(256);
}

void OrderScreen::Init() {
    // Optional: load UI state if needed
}

void OrderScreen::OnExit() {
    // Optional cleanup
}

void OrderScreen::Render(float dt) {
    ImGui::Text("Restaurant Order Management");
    ImGui::Separator();

    DrawBackButton();
    DrawNewOrderInput();
    ImGui::Spacing();

    DrawOrderTable();
    ImGui::Spacing();

    DrawSaveButton();
    ImGui::SameLine();

    // Handle popups
    if (m_ShowMealSelectionPopup) {
        DrawMealSelectionPopup();
    }

    if (m_ShowMealDetailsForOrder != -1) {
        DrawMealDetailsPopup();
    }

    if (m_ShowMealEditPopup) {
        DrawMealEditPopup();
    }
}

// Draw input + button to add new order
void OrderScreen::DrawNewOrderInput() {
    ImGui::Text("Create New Order");
    ImGui::Separator();

    // Table ID input
    if (ImGui::InputText("Table ID", m_NewTableIDInput.data(), m_NewTableIDInput.size())) {
        m_NewTableIDStr = std::string(m_NewTableIDInput.data());
    }

    // Customer Phone input
    if (ImGui::InputText("Customer Phone", m_NewCustomerPhoneInput.data(), m_NewCustomerPhoneInput.size())) {
        m_NewCustomerPhoneStr = std::string(m_NewCustomerPhoneInput.data());
    }

    // Select Meals button
    if (ImGui::Button("Select Meals")) {
        m_ShowMealSelectionPopup = true;
        // Initialize meal selection state when opening popup
        auto allMeals = m_MealRepo->GetMeals();
        m_MealSelectionState.resize(allMeals.size(), false);
        std::fill(m_MealSelectionState.begin(), m_MealSelectionState.end(), false);
    }

    ImGui::SameLine();
    // Show selected meals summary with quantities
    if (m_SelectedMealQuantities.empty()) {
        ImGui::Text("Selected: 0 meals");
    } else {
        int totalMeals = 0;
        for (const auto &[mealID, quantity]: m_SelectedMealQuantities) {
            totalMeals += quantity;
        }
        ImGui::Text("Selected: %d meals (%zu unique)", totalMeals, m_SelectedMealQuantities.size());
    }

    // Create Order button
    if (ImGui::Button("Create Order")) {
        // Debug output
        printf("Debug: Attempting to create order - TableID='%s', Phone='%s', Selected meals=%zu\n",
               m_NewTableIDStr.c_str(), m_NewCustomerPhoneStr.c_str(), m_SelectedMealIDs.size());

        if (!m_NewTableIDStr.empty() && !m_NewCustomerPhoneStr.empty() && !m_SelectedMealIDs.empty()) {
            int tableID = std::stoi(m_NewTableIDStr);
            std::vector<std::shared_ptr<Meal> > selectedMeals;

            for (int mealID: m_SelectedMealIDs) {
                auto meal = m_MealRepo->GetMealByID(mealID);
                if (meal) {
                    selectedMeals.push_back(meal);
                    printf("Debug: Added meal ID %d to order\n", mealID);
                } else {
                    printf("Debug: Meal ID %d not found!\n", mealID);
                }
            }

            if (!selectedMeals.empty()) {
                m_Manager->CreateOrder(tableID, m_NewCustomerPhoneStr, DateTime::Now(), selectedMeals);
                printf("Debug: Order created successfully!\n");

                // Clear inputs
                m_NewTableIDStr.clear();
                m_NewCustomerPhoneStr.clear();
                std::fill(m_NewTableIDInput.begin(), m_NewTableIDInput.end(), '\0');
                std::fill(m_NewCustomerPhoneInput.begin(), m_NewCustomerPhoneInput.end(), '\0');
                m_SelectedMealIDs.clear();
                m_SelectedMealQuantities.clear();
            } else {
                printf("Debug: No valid meals found for selected IDs!\n");
            }
        } else {
            printf("Debug: Missing required fields - TableID empty: %s, Phone empty: %s, No meals: %s\n",
                   m_NewTableIDStr.empty() ? "YES" : "NO",
                   m_NewCustomerPhoneStr.empty() ? "YES" : "NO",
                   m_SelectedMealIDs.empty() ? "YES" : "NO");
        }
    }
}

// Draw table showing all orders and allow edit/delete
void OrderScreen::DrawOrderTable() {
    const auto orders = m_Manager->GetAllOrders();

    if (ImGui::BeginTable("Orders", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Order ID", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Table ID", ImGuiTableColumnFlags_WidthFixed, 80.0f);
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
            ImGui::SetNextItemWidth(-1);
            ImGui::InputInt(("##table" + std::to_string(orderID)).c_str(), &tableID);

            // --- Customer Phone ---
            ImGui::TableSetColumnIndex(2);
            std::string &customerPhone = m_EditedCustomerPhone[orderID];
            if (customerPhone.empty()) customerPhone = order->GetCustomerPhone();

            char phoneBuffer[256];
            strncpy(phoneBuffer, customerPhone.c_str(), sizeof(phoneBuffer) - 1);
            phoneBuffer[sizeof(phoneBuffer) - 1] = '\0';
            ImGui::SetNextItemWidth(-1);
            if (ImGui::InputText(("##phone" + std::to_string(orderID)).c_str(), phoneBuffer, sizeof(phoneBuffer))) {
                customerPhone = std::string(phoneBuffer);
            }

            // --- Time (Hours and Minutes only) ---
            ImGui::TableSetColumnIndex(3);

            if (m_EditedDateTime.find(orderID) == m_EditedDateTime.end()) {
                m_EditedDateTime[orderID] = order->GetDate();
            }
            DateTime &dt = m_EditedDateTime[orderID];

            auto hours = DateTime::GetValidHours();
            auto minutes = DateTime::GetValidMinutes(5);

            std::string hourLabel = (dt.GetHour() < 10 ? "0" : "") + std::to_string(dt.GetHour());
            ImGui::SetNextItemWidth(50);
            if (ImGui::BeginCombo(("##hour_" + std::to_string(orderID)).c_str(), hourLabel.c_str())) {
                for (int h: hours) {
                    std::string label = (h < 10 ? "0" : "") + std::to_string(h);
                    bool selected = (h == dt.GetHour());
                    if (ImGui::Selectable(label.c_str(), selected)) {
                        dt.SetHour(h);
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            ImGui::SameLine();
            ImGui::Text(":");
            ImGui::SameLine();

            std::string minuteLabel = (dt.GetMinute() < 10 ? "0" : "") + std::to_string(dt.GetMinute());
            ImGui::SetNextItemWidth(50);
            if (ImGui::BeginCombo(("##minute_" + std::to_string(orderID)).c_str(), minuteLabel.c_str())) {
                for (int m: minutes) {
                    std::string label = (m < 10 ? "0" : "") + std::to_string(m);
                    bool selected = (m == dt.GetMinute());
                    if (ImGui::Selectable(label.c_str(), selected)) {
                        dt.SetMinute(m);
                    }
                    if (selected) ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
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
                m_ShowMealEditPopup = true;
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
    // Always try to open the popup when the flag is set
    if (m_ShowMealSelectionPopup) {
        ImGui::OpenPopup("Select Meals");
    }

    // Handle the modal popup
    if (ImGui::BeginPopupModal("Select Meals", &m_ShowMealSelectionPopup, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Select meals for the new order:");
        ImGui::Separator();

        // Get all available meals
        auto allMeals = m_MealRepo->GetMeals();

        // Debug output
        printf("Debug: Found %zu meals in repository\n", allMeals.size());

        // Ensure selection state vector is properly sized
        if (m_MealSelectionState.size() != allMeals.size()) {
            m_MealSelectionState.resize(allMeals.size(), false);
        }

        if (allMeals.empty()) {
            ImGui::Text("No meals available in the system.");
        } else {
            if (ImGui::BeginTable("MealSelection", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                ImGui::TableSetupColumn("Select");
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Price");
                ImGui::TableSetupColumn("Items Count");
                ImGui::TableSetupColumn("Quantity");
                ImGui::TableHeadersRow();

                for (size_t i = 0; i < allMeals.size(); ++i) {
                    const auto &meal = allMeals[i];
                    int mealID = meal->GetID();
                    ImGui::TableNextRow();

                    ImGui::TableSetColumnIndex(0);
                    bool isSelected = m_MealSelectionState[i];
                    if (ImGui::Checkbox(("##select" + std::to_string(i)).c_str(), &isSelected)) {
                        m_MealSelectionState[i] = isSelected;
                        // Initialize quantity to 1 when first selected
                        if (isSelected && m_MealQuantities.find(mealID) == m_MealQuantities.end()) {
                            m_MealQuantities[mealID] = 1;
                        }
                        printf("Debug: Meal %d selection changed to %s\n", mealID, isSelected ? "true" : "false");
                    }

                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d", mealID);

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text("$%.2f", meal->GetPrice());

                    ImGui::TableSetColumnIndex(3);
                    ImGui::Text("%zu", meal->GetMealItems().size());

                    ImGui::TableSetColumnIndex(4);
                    // Quantity input - only enabled if meal is selected
                    if (m_MealQuantities.find(mealID) == m_MealQuantities.end()) {
                        m_MealQuantities[mealID] = 1; // Initialize to 1
                    }

                    int &quantity = m_MealQuantities[mealID];
                    ImGui::BeginDisabled(!isSelected);
                    ImGui::SetNextItemWidth(60);
                    ImGui::InputInt(("##qty" + std::to_string(i)).c_str(), &quantity, 1, 1);
                    if (quantity < 1) quantity = 1; // Ensure minimum quantity of 1
                    ImGui::EndDisabled();
                }

                ImGui::EndTable();
            }
        }

        ImGui::Separator();

        if (ImGui::Button("Confirm Selection")) {
            m_SelectedMealIDs.clear();
            m_SelectedMealQuantities.clear();
            for (size_t i = 0; i < m_MealSelectionState.size() && i < allMeals.size(); ++i) {
                if (m_MealSelectionState[i]) {
                    int mealID = allMeals[i]->GetID();
                    int quantity = m_MealQuantities[mealID];

                    // Add multiple copies of the meal based on quantity
                    for (int q = 0; q < quantity; ++q) {
                        m_SelectedMealIDs.push_back(mealID);
                    }
                    m_SelectedMealQuantities[mealID] = quantity;
                    printf("Debug: Selected meal ID %d with quantity %d\n", mealID, quantity);
                }
            }
            printf("Debug: Total meal instances selected: %zu\n", m_SelectedMealIDs.size());
            m_ShowMealSelectionPopup = false;
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            // Clear all selection data
            std::fill(m_MealSelectionState.begin(), m_MealSelectionState.end(), false);
            m_MealQuantities.clear();
            m_ShowMealSelectionPopup = false;
        }

        ImGui::EndPopup();
    }
}

void OrderScreen::DrawMealDetailsPopup() {
    if (m_ShowMealDetailsForOrder != -1) {
        std::string popupName = "Order Meals Details##" + std::to_string(m_ShowMealDetailsForOrder);
        ImGui::OpenPopup(popupName.c_str());
    }

    std::string popupName = "Order Meals Details##" + std::to_string(m_ShowMealDetailsForOrder);
    bool shouldClose = false;

    if (ImGui::BeginPopupModal(popupName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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

            auto meals = targetOrder->GetMeals();

            if (meals.empty()) {
                ImGui::Text("No meals in this order.");
            } else {
                if (ImGui::BeginTable("MealDetails", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                    ImGui::TableSetupColumn("Meal ID");
                    ImGui::TableSetupColumn("Item Name");
                    ImGui::TableSetupColumn("Quantity");
                    ImGui::TableSetupColumn("Addons");
                    ImGui::TableSetupColumn("Price");
                    ImGui::TableHeadersRow();

                    for (const auto &meal: meals) {
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
                                ImGui::Text("Item ID: %d", item->GetMenuItem().GetID());

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
                    }

                    ImGui::EndTable();
                }
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

void OrderScreen::DrawMealEditPopup() {
    if (m_ShowMealEditPopup && m_EditingOrderID != -1) {
        std::string popupName = "Edit Order Meals##" + std::to_string(m_EditingOrderID);
        ImGui::OpenPopup(popupName.c_str());
    }

    std::string popupName = "Edit Order Meals##" + std::to_string(m_EditingOrderID);
    bool shouldClose = false;

    if (ImGui::BeginPopupModal(popupName.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        auto orders = m_Manager->GetAllOrders();
        std::shared_ptr<Order> targetOrder = nullptr;

        for (const auto &order: orders) {
            if (order->GetID() == m_EditingOrderID) {
                targetOrder = order;
                break;
            }
        }

        if (targetOrder) {
            ImGui::Text("Editing Order ID: %d", targetOrder->GetID());
            ImGui::Separator();

            if (ImGui::Button("Add Meal from Menu")) {
                ImGui::OpenPopup("Add Meal Popup");
            }

            ImGui::Separator();

            auto meals = targetOrder->GetMeals();

            if (meals.empty()) {
                ImGui::Text("No meals in this order. Use 'Add Meal from Menu' to add meals.");
            } else {
                if (ImGui::BeginTable("EditMeals", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                    ImGui::TableSetupColumn("Meal ID");
                    ImGui::TableSetupColumn("Price");
                    ImGui::TableSetupColumn("Items");
                    ImGui::TableSetupColumn("Actions");
                    ImGui::TableHeadersRow();

                    std::vector<int> mealsToRemove;

                    for (const auto &meal: meals) {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", meal->GetID());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("$%.2f", meal->GetPrice());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%zu items", meal->GetMealItems().size());

                        ImGui::TableSetColumnIndex(3);
                        if (ImGui::Button(("Remove##meal" + std::to_string(meal->GetID())).c_str())) {
                            mealsToRemove.push_back(meal->GetID());
                        }
                    }

                    ImGui::EndTable();

                    // Actually remove the meals that were marked for removal
                    for (int mealID: mealsToRemove) {
                        printf("Debug: Removing meal ID %d from order %d\n", mealID, targetOrder->GetID());
                        bool removed = targetOrder->RemoveMeal(mealID);
                        if (removed) {
                            printf("Debug: Successfully removed meal %d\n", mealID);
                        } else {
                            printf("Debug: Failed to remove meal %d (not found)\n", mealID);
                        }
                    }
                }
            }

            // Handle Add Meal Popup with actual functionality
            if (ImGui::BeginPopupModal("Add Meal Popup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
                ImGui::Text("Select meal to add to order:");
                ImGui::Separator();

                auto allMeals = m_MealRepo->GetMeals();

                if (ImGui::BeginTable("AddMealTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
                    ImGui::TableSetupColumn("Meal ID");
                    ImGui::TableSetupColumn("Price");
                    ImGui::TableSetupColumn("Items");
                    ImGui::TableSetupColumn("Action");
                    ImGui::TableHeadersRow();

                    for (const auto &meal: allMeals) {
                        ImGui::TableNextRow();

                        ImGui::TableSetColumnIndex(0);
                        ImGui::Text("%d", meal->GetID());

                        ImGui::TableSetColumnIndex(1);
                        ImGui::Text("$%.2f", meal->GetPrice());

                        ImGui::TableSetColumnIndex(2);
                        ImGui::Text("%zu items", meal->GetMealItems().size());

                        ImGui::TableSetColumnIndex(3);
                        if (ImGui::Button(("Add##meal" + std::to_string(meal->GetID())).c_str())) {
                            printf("Debug: Adding meal ID %d to order %d\n", meal->GetID(), targetOrder->GetID());
                            targetOrder->AddMeal(meal);
                            printf("Debug: Meal added successfully\n");
                            ImGui::CloseCurrentPopup();
                        }
                    }

                    ImGui::EndTable();
                }

                ImGui::Separator();
                if (ImGui::Button("Cancel")) {
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        } else {
            ImGui::Text("Order not found!");
        }

        ImGui::Separator();
        if (ImGui::Button("Done")) {
            shouldClose = true;
        }

        ImGui::EndPopup();
    }

    if (shouldClose || !ImGui::IsPopupOpen(popupName.c_str())) {
        m_EditingOrderID = -1;
        m_ShowMealEditPopup = false;
    }
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
