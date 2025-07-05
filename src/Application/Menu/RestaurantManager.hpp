#pragma once
#include "Menu/Branch.hpp"
class JsonHandle;

class RestaurantManager {
private:
    RestaurantManager();

    ~RestaurantManager() = default;

    std::unique_ptr<Restaurant> m_Restaurant;
    std::unique_ptr<JsonHandle> m_FileHandle;
    bool m_IsLoaded;

public:
    RestaurantManager(const RestaurantManager &) = delete;

    RestaurantManager &operator=(const RestaurantManager &) = delete;

    static RestaurantManager &GetInstance();

    const Restaurant &GetRestaurant() const;

    void LoadRestaurantData(const std::string &filePath);

    void SaveRestaurantData(const std::string &filePath) const;

    bool IsLoaded() const;
};
