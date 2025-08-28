#pragma once

#include <memory>

#include "Screen.hpp"

class MenuManager;
class BudgetManager;
class ReservationManager;
class IMealRepository;
class OrderManager;
class IMenuRepository;

class Dashboard : public Screen {
public:
    Dashboard(Core &core);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;

private:
    std::shared_ptr<OrderManager> m_OrderManager;
    std::shared_ptr<IMealRepository> m_MealRepo;
    std::shared_ptr<ReservationManager> m_ReservationManager;
    std::shared_ptr<BudgetManager> m_BudgetManager;
    std::shared_ptr<MenuManager> m_MenuManager;

    // Navigation methods
    void NavigateToOrderScreen();

    void NavigateToMenuScreen();

    void NavigateToBudgetScreen();

    void NavigateToReservationScreen();

    void NavigateToStorageScreen();
};
