#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Order.hpp"
#include "IMealRepository.hpp"

class OrderRepository {
private:
    std::string m_FilePath;
    std::unique_ptr<JsonHandle> m_FileHandler;
    std::shared_ptr<IMealRepository> m_MealRepo;

public:
    OrderRepository(const std::string &FilePath, const std::shared_ptr<IMealRepository> &MealRepo);

    std::vector<std::shared_ptr<Order> > LoadAllOrders();

    void SaveAllOrders(const std::vector<std::shared_ptr<Order> > &Orders);
};
