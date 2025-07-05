#pragma once
#include <memory>
#include <vector>

#include "Restaurant/Restaurant.hpp"

class Restaurant;

class IRestaurantRepository
{
public:
    virtual ~IRestaurantRepository() = default;

    virtual void SaveRestaurantData(std::string filePath) const = 0;

    void SaveRestaurant(const std::unique_ptr<Restaurant> &restaurant);

    Restaurant &GetRestaurant() const;

    std::vector<std::shared_ptr<Branch>> GetBranches() const;

    std::shared_ptr<Branch> GetBranchByID(int BranchID) const;

    std::shared_ptr<Branch> GetBranchByName(const std::string &BranchName) const;

    std::shared_ptr<Branch> SaveBranch(const std::shared_ptr<Branch> &branch);

    void RemoveBranch(const std::shared_ptr<Branch> &branch);

    void RemoveBranch(int BranchID);

private:
    std::unique_ptr<Restaurant> m_Restaurant;
};