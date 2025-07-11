#pragma once

#include "OrderRepository.hpp"
#include "FileMealRepository.hpp" 
#include "FileMenuRepository.hpp"
#include "Logger.hpp"
#include "IDManager.hpp"
#include <iostream>

void OrderTest() {
    FileMenuAddonRepository menuAddonRepo("Data/MenuAddons.json"); 
    FileMenuItemRepository menuItemRepo("Data/MenuItem.json", menuAddonRepo);
    FileMenuRepository menuRepo("Data/Menu.json", menuItemRepo);
    FileMealRepository mealRepo("Data/MealHistory.json", menuItemRepo, menuAddonRepo);

    OrderRepository repo("Data/Order.json", mealRepo);

    // Load existing orders
    std::vector<std::shared_ptr<Order>> orders = repo.LoadAllOrders();
    std::cout << "Loaded Orders: " << orders.size() << std::endl;

    for (const auto& order : orders) {
        std::cout << "Order #" << order->GetID()
                  << " | Table: " << order->GetTableID()
                  << " | Customer: " << order->GetCustomerPhone()
                  << " | Total: " << order->GetTotalPrice()
                  << " | Status: " << OrderStatusToString(order->GetStatus())
                  << std::endl;
    }

    // --- Add dummy order for testing ---
    int newID = IDManager::GetInstance().GetNextID("Order");
    DateTime now = DateTime::Now();
    auto newOrder = std::make_shared<Order>(newID, 5, "0912345678", now);
    newOrder->SetOrderStatus(OrderStatus::Received);

    auto dummyMeal = mealRepo.GetMealByID(1); // Test with an existing ID
    if (dummyMeal) {
        newOrder->AddMeal(dummyMeal);
    }
    dummyMeal = mealRepo.GetMealByID(3);
    if (dummyMeal) {
        newOrder->AddMeal(dummyMeal);
    }
    orders.push_back(newOrder);

    // Save back all orders
    repo.SaveAllOrders(orders);
    std::cout << "Test order added and all orders saved successfully.\n";
}