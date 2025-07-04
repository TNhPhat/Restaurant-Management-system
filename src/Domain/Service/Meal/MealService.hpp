#pragma once
#include <memory>
#include <vector>

#include "Meal.hpp"

class MealService {
public:
    ~MealService() = default;

    void ParseMealFromOrder();
};
