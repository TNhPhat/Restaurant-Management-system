#include "MealIngredients.hpp"
#include <stdexcept>

MealIngredient::MealIngredient(const std::string &name, const int quantity) : Name(name), Quantity(quantity)
{
    if (Name.empty())
    {
        throw std::invalid_argument("Ingredient name cannot be empty");
    }
    if (Quantity <= 0)
    {
        throw std::invalid_argument("Ingredient quantity must be greater than zero");
    }
}

MealIngredient::MealIngredient(const std::pair<std::string, int> &ingredient) : Name(ingredient.first),
                                                                                Quantity(ingredient.second)
{
    if (Name.empty())
    {
        throw std::invalid_argument("Ingredient name cannot be empty");
    }
    if (Quantity <= 0)
    {
        throw std::invalid_argument("Ingredient quantity must be greater than zero");
    }
}