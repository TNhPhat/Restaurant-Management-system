#include "RestaurantManager.hpp"

#include "MenuManager.hpp"

RestaurantManager &RestaurantManager::GetInstance()
{
    static RestaurantManager instance;
    return instance;
}

const Restaurant &RestaurantManager::GetRestaurant() const
{
    return *m_Restaurant;
}

void RestaurantManager::LoadRestaurantData(const std::string &filePath)
{
    if (this->IsLoaded() == true)
        return;
    if (MenuManager::GetInstance().IsLoaded() == false)
    {
        throw std::runtime_error("Menu data must be loaded before loading restaurant data.");
    }
    m_FileHandle->LoadFile(filePath);
    json data = m_FileHandle->GetDaTa();
    m_Restaurant = std::make_unique<Restaurant>(data.at("Restaurant Name").get<std::string>());
    for (const auto &branchData : data.at("Branches"))
    {
        int id = branchData.at("Branch ID").get<int>();
        auto name = branchData.at("Branch Name").get<std::string>();
        auto location = branchData.at("Address").get<std::string>();
        auto menu = MenuManager::GetInstance().GetMenuByID(branchData.at("Menu ID").get<int>());
        m_Restaurant->AddBranch(id, name, location, menu);
    }
    m_IsLoaded = true;
}

void RestaurantManager::SaveRestaurantData(const std::string &filePath) const
{
    SetKeyCommand setCommand({"Restaurant Name"}, m_Restaurant->GetName());
    m_FileHandle->ExecuteCommand(&setCommand);
    m_FileHandle->ExecuteCommand(new SetKeyCommand({"Number of Branches"}, this->m_Restaurant->GetBranches().size()));
    json branchesArray = json::array();
    for (const auto &branch : m_Restaurant->GetBranches())
    {
        json branchData = {
            {"Branch ID", branch->GetID()},
            {"Branch Name", branch->GetName()},
            {"Address", branch->GetLocation()},
            {"Menu ID", branch->GetMenu()->GetID()}};
        branchesArray.push_back(branchData);
    }
    m_FileHandle->ExecuteCommand(
        new SetKeyCommand({"Branches"}, branchesArray));
    m_FileHandle->SaveFileAs(filePath);
    return;
}

bool RestaurantManager::IsLoaded() const
{
    return m_IsLoaded;
}

RestaurantManager::RestaurantManager() : m_IsLoaded(false)
{
    m_FileHandle = std::make_unique<JsonHandle>();
}
