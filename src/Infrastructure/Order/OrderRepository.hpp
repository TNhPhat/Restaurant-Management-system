#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Order.hpp"
#include "IMealRepository.hpp"

class OrderRepository {
private:
    std::string m_FilePath;
    JsonHandle* m_FileHandler;
    const IMealRepository& m_MealRepo;

public:
    OrderRepository(const std::string& FilePath, JsonHandle* FileHandler, const IMealRepository& MealRepo);

    std::vector<std::shared_ptr<Order>> LoadAllOrders();
    void SaveAllOrders(const std::vector<std::shared_ptr<Order>>& Orders);
};
