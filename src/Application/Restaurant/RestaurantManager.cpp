#include "../Restaurant/RestaurantManager.hpp"

RestaurantManager::RestaurantManager(FileRestaurantRepository &restaurantRepository)
    : m_RestaurantRepository(restaurantRepository) {
}

Restaurant &RestaurantManager::GetRestaurant() const {
    return m_RestaurantRepository.GetRestaurant();
}
