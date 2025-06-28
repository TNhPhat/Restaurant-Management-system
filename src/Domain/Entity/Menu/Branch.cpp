#include "Branch.hpp"
#include "../../../Core/Utils/IDManager.hpp"

Branch::Branch(std::string Name, std::string Location, const Menu &menu) : m_Name(Name), m_Location(Location), m_BranchID(IDManager::GetInstance().GetNextID("Branch")), m_Menu(menu)
{
}

int Branch::GetID() const
{
    return this->m_BranchID;
}

std::string Branch::GetName() const
{
    return this->m_Name;
}

std::string Branch::GetLocation() const
{
    return this->m_Location;
}

void Branch::SetName(std::string Name)
{
    this->m_Name = Name;
}

void Branch::SetLocation(std::string Location)
{
    this->m_Location = Location;
}

Restaurant::Restaurant(std::string Name) : m_Name(Name)
{
}

template <typename... Args>
void Restaurant::AddBranch(Args &&...args)
{
    AddBranch(std::make_shared<const Branch>(std::forward<Args>(args)...));
}

void Restaurant::AddBranch(std::shared_ptr<const Branch> branch)
{
    this->m_Branches.emplace_back(branch);
}

const std::vector<std::shared_ptr<const Branch>> Restaurant::GetBranches() const
{
    return this->m_Branches;
}