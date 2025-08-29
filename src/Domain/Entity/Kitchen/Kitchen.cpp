#include "Kitchen.hpp"

Kitchen::Kitchen(int n_KitchenID, Storage &n_Storage)
    : m_KitchenID(n_KitchenID), m_Storage(n_Storage) {
    m_Orders.clear();
}

bool Kitchen::AddOrder(Order n_Order) {
    auto Meals = n_Order.GetMeal();
    auto MealItems = Meals->GetMealItems();
    for (auto const mealitem: MealItems) {
        int ID = mealitem->GetID();
        int Q = mealitem->GetQuantity();

        // need to edit
        Resource *RS = this->m_Storage.SearchByID(ID);
        int RSQ = RS->GetQuantity();
        if (Q > RSQ) {
            n_Order.SetOrderStatus(OrderStatus::Canceled);
            return false;
        }
    }

    for (auto const mealitem: MealItems) {
        int ID = mealitem->GetID();
        int Q = mealitem->GetQuantity();

        // need to edit
        Resource *RS = this->m_Storage.SearchByID(ID);
        int RSQ = RS->GetQuantity();
        RS->SetQuantity(RSQ - Q);
    }
    return true;
}
