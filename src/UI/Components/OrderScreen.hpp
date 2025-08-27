#pragma once

#include "Screen.hpp"
#include "../../Application/Order/OrderManager.hpp"
#include "../../Infrastructure/Order/OrderRepository.hpp"
#include "../../Domain/Entity/Order/OrderStatus.hpp"
#include "../../Infrastructure/FileHandle/FileHandle.hpp"
#include "../../Core/Utils/IDManager.hpp"
#include "../../Infrastructure/Meal/FileMealRepository.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class OrderScreen : public Screen {
private:
    std::shared_ptr<OrderManager> m_Manager;
    std::shared_ptr<IMealRepository> m_MealRepo;

    // New order inputs
    std::vector<char> m_NewTableIDInput;
    std::string m_NewTableIDStr;
    std::vector<char> m_NewCustomerPhoneInput;
    std::string m_NewCustomerPhoneStr;

    // Editing state
    std::unordered_map<int, int> m_EditedTableID;
    std::unordered_map<int, DateTime> m_EditedDateTime;
    std::unordered_map<int, OrderStatus> m_EditedStatus;
    std::unordered_map<int, std::string> m_EditedCustomerPhone;

    // Meal selection for new orders
    std::vector<int> m_SelectedMealIDs;
    std::unordered_map<int, int> m_SelectedMealQuantities; // NEW: Track quantities for each meal
    bool m_ShowMealSelectionPopup = false;
    std::vector<bool> m_MealSelectionState;
    std::unordered_map<int, int> m_MealQuantities; // NEW: Track quantities in selection popup

    // Meal detail popup
    int m_ShowMealDetailsForOrder = -1;

    // Meal editing popup
    int m_EditingOrderID = -1;
    bool m_ShowMealEditPopup = false;

public:
    OrderScreen(Core &core, const std::shared_ptr<OrderManager> &manager,
                const std::shared_ptr<IMealRepository> &mealRepo);

    void Init() override;

    void OnExit() override;

    void Render(float dt) override;

private:
    void DrawNewOrderInput();

    void DrawOrderTable();

    void DrawSaveButton();

    void DrawBackButton();

    void DrawMealSelectionPopup();

    void DrawMealDetailsPopup();

    void DrawMealEditPopup();

    const char *StatusToString(OrderStatus status);

    std::vector<OrderStatus> GetValidOrderStatuses();
};
