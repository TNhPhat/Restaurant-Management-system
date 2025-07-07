#pragma once
#include <string>

struct MealIngredient {
    std::string Name;
    int Quantity;

    MealIngredient() = default;

    MealIngredient(const std::string &name, int quantity);

    explicit MealIngredient(const std::pair<std::string, int> &ingredient);
};
