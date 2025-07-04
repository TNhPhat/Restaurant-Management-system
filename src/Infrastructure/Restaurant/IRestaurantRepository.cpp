#include "IRestaurantRepository.hpp"

void IRestaurantRepository::SaveRestaurant(const std::unique_ptr<Restaurant> &restaurant)
{
    if (restaurant == nullptr)
    {
        LOG_ERROR("Error saving Restaurant: Restaurant is null.");
        return;
    }
    this->m_Restaurant.reset();
    this->m_Restaurant = std::make_unique<Restaurant>(*restaurant);
}

Restaurant &IRestaurantRepository::GetRestaurant() const
{
    if (this->m_Restaurant == nullptr)
    {
        LOG_ERROR("Restaurant is not initialized.");
        throw std::runtime_error("Restaurant is not initialized.");
    }
    return *this->m_Restaurant;
}

std::vector<std::shared_ptr<Branch>> IRestaurantRepository::GetBranches() const
{
    return this->m_Restaurant->GetBranches();
}

std::shared_ptr<Branch> IRestaurantRepository::GetBranchByID(int BranchID) const
{
    for (const auto &branch : this->m_Restaurant->GetBranches())
    {
        if (branch->GetID() == BranchID)
        {
            return branch;
        }
    }
    return nullptr;
}

std::shared_ptr<Branch> IRestaurantRepository::GetBranchByName(const std::string &BranchName) const
{
    for (const auto &branch : this->m_Restaurant->GetBranches())
    {
        if (branch->GetName() == BranchName)
        {
            return branch;
        }
    }
    return nullptr;
}

std::shared_ptr<Branch> IRestaurantRepository::SaveBranch(const std::shared_ptr<Branch> &branch)
{
    if (branch == nullptr)
    {
        LOG_ERROR("Error saving Branch: Branch is null.");
        return nullptr;
    }
    this->m_Restaurant->AddBranch(branch);
    return branch;
}

void IRestaurantRepository::RemoveBranch(const std::shared_ptr<Branch> &branch)
{
    if (branch == nullptr)
    {
        LOG_ERROR("Error removing Branch: Branch is null.");
        return;
    }
    for (const auto &existingBranch : this->m_Restaurant->GetBranches())
    {
        if (existingBranch->GetID() == branch->GetID())
        {
            this->m_Restaurant->RemoveBranch(existingBranch->GetID());
            return;
        }
    }
    LOG_ERROR("Branch with ID {} not found.", branch->GetID());
}

void IRestaurantRepository::RemoveBranch(int BranchID)
{
    for (const auto &branch : this->m_Restaurant->GetBranches())
    {
        if (branch->GetID() == BranchID)
        {
            this->m_Restaurant->RemoveBranch(BranchID);
            return;
        }
    }
    LOG_ERROR("Branch with ID {} not found.", BranchID);
}
