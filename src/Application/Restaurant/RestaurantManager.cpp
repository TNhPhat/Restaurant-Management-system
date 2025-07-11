#include "../Restaurant/RestaurantManager.hpp"

RestaurantManager::RestaurantManager(FileRestaurantRepository &restaurantRepository)
    : m_RestaurantRepository(restaurantRepository) {
}

Restaurant &RestaurantManager::GetRestaurant() const {
    return m_RestaurantRepository.GetRestaurant();
}

std::string RestaurantManager::GetRestaurantName() const {
    return this->GetRestaurant().GetName();
}

std::string RestaurantManager::GetRestaurantAddress() const {
    return this->GetRestaurant().GetBranches()[0]->GetLocation();
}
