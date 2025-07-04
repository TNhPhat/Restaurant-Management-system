#pragma once
#include "../../Infrastructure/Meal/FileMealRepository.hpp"

class MealManager {
private:
    FileMealRepository &m_MealRepository;

public:
    MealManager(FileMealRepository &MealRepository);

    ~MealManager() = default;
};
