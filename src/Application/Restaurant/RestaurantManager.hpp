#pragma once
#include "../../Domain/Entity/Restaurant/Restaurant.hpp"
#include "Restaurant/FileRestaurantRepository.hpp"

class RestaurantManager {
private:
    FileRestaurantRepository &m_RestaurantRepository;

public:
    RestaurantManager(FileRestaurantRepository &restaurantRepository);

    ~RestaurantManager() = default;

    Restaurant &GetRestaurant() const;

    std::string GetRestaurantName() const;

    std::string GetRestaurantAddress() const;
};
