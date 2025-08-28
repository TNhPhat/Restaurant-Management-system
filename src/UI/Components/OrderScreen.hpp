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

#include "DateTimePicker.hpp"

class Table;
class TableManager;

class OrderScreen : public Screen {
private:
    std::shared_ptr<OrderManager> m_Manager;
    std::shared_ptr<const IMenuRepository> m_MenuRepo;
    std::shared_ptr<IMealRepository> m_MealRepo;
    std::shared_ptr<TableManager> m_TableManager;


    // New order inputs
    int m_NewTableID = -1;
    std::string m_NewCustomerPhoneStr;
    DateTime m_NewDate;

    // Editing state
    std::unordered_map<int, int> m_EditedTableID;
    std::unordered_map<int, DateTime> m_EditedDateTime;
    std::unordered_map<int, OrderStatus> m_EditedStatus;
    std::unordered_map<int, std::string> m_EditedCustomerPhone;

    // Meal selection for new orders
    std::vector<int> m_SelectedMealIDs;
    std::unordered_map<int, int> m_SelectedMenuItemQuantities; // NEW: Track quantities for each meal
    bool m_ShowMealSelectionPopup = false;
    std::vector<bool> m_MealSelectionState;
    std::unordered_map<int, int> m_MealQuantities; // NEW: Track quantities in selection popup
    std::vector<std::shared_ptr<Table> > m_AvailableTables;
    std::unordered_map<int, std::unordered_map<int, int> > m_AddonsMap;

    DateTimePicker m_DatePicker;

    // Meal detail popup
    int m_ShowMealDetailsForOrder = -1;

    // Meal editing popup
    int m_EditingOrderID = -1;
    bool m_ShowMealEditPopup = false;
    int m_SelectedMenuID = -1;
    int m_SelectedMenuSectionID = -1;
    bool m_AddonEditPopup = false;
    bool m_confirm = false;
    int m_OrderID = -1;
    int m_MealID = -1;

public:
    OrderScreen(Core &core, std::shared_ptr<OrderManager> manager, std::shared_ptr<const IMenuRepository> menuRepo,
                std::shared_ptr<IMealRepository> mealRepo);

    void Init() override;

    void OnExit() override;

    void Render(float dt) override;

private:
    void DrawOrderTable();

    void DrawSaveButton();

    void DrawBackButton();

    void DrawMealSelectionPopup();

    void DrawMealDetailsPopup();

    void DrawNewOrderButton();

    void DrawEditAddonPopup(int menuID);

    static const char *StatusToString(OrderStatus status);

    static std::vector<OrderStatus> GetValidOrderStatuses();
};
