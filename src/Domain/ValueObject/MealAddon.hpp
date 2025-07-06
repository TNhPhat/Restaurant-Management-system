#pragma once
#include <memory>

#include "../Entity/Menu/Menu.hpp"

class MealAddon {
private:
    std::shared_ptr<MenuAddon> m_Addon;
    unsigned int m_Quantity;

public:
    MealAddon(const std::shared_ptr<MenuAddon> &Addon, int Quantity);

    std::shared_ptr<MenuAddon> GetAddon() const;

    int GetQuantity() const;

    void SetQuantity(unsigned int Quantity);

    void AddQuantity(unsigned int Quantity);

    void RemoveQuantity(unsigned int Quantity);

    std::vector<MealIngredient> GetIngredients() const;
};
