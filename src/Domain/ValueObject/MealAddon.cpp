#include "MealAddon.hpp"

MealAddon::MealAddon(const std::shared_ptr<MenuAddon> &Addon,
                     const int Quantity) : m_Addon(Addon), m_Quantity(Quantity) {
    if (this->m_Addon == nullptr) {
        throw std::invalid_argument("Addon cannot be null");
    }
    if (this->m_Quantity <= 0) {
        throw std::invalid_argument("Quantity must be greater than zero");
    }
}

std::shared_ptr<MenuAddon> MealAddon::GetAddon() const {
    return this->m_Addon;
}

int MealAddon::GetQuantity() const {
    return this->m_Quantity;
}

void MealAddon::SetQuantity(const unsigned int Quantity) {
    this->m_Quantity = Quantity;
}

void MealAddon::AddQuantity(const unsigned int Quantity) {
    this->m_Quantity += Quantity;
}

void MealAddon::RemoveQuantity(const unsigned int Quantity) {
    this->m_Quantity -= Quantity;
}

std::vector<MealIngredient> MealAddon::GetIngredients() const {
    std::vector<MealIngredient> ingredients = this->m_Addon->GetIngredients();
    for (auto &[Name, Quantity]: ingredients) {
        Quantity *= this->m_Quantity;
    }
    return ingredients;
}
