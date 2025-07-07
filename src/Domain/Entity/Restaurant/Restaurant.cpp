#include "Restaurant.hpp"
#include "IDManager.hpp"

Branch::Branch(const std::string &Name, const std::string &Location, const std::shared_ptr<Menu> &Menu) : Branch(
    IDManager::GetInstance().GetNextID("Branch"), Name, Location, Menu) {
}

Branch::Branch(const int ID, const std::string &Name, const std::string &Location,
               const std::shared_ptr<Menu> &menu) : m_BranchID(ID),
                                                    m_Name(Name), m_Location(Location), m_Menu(menu) {
    if (ID <= 0) {
        throw std::invalid_argument("Branch ID must be greater than zero");
    }
    if (Name.empty()) {
        throw std::invalid_argument("Branch name cannot be empty");
    }
    if (Location.empty()) {
        throw std::invalid_argument("Branch location cannot be empty");
    }
}

int Branch::GetID() const {
    return this->m_BranchID;
}

std::string Branch::GetName() const {
    return this->m_Name;
}

std::string Branch::GetLocation() const {
    return this->m_Location;
}

const std::shared_ptr<Menu> &Branch::GetMenu() const {
    return this->m_Menu;
}

void Branch::SetName(const std::string &Name) {
    this->m_Name = Name;
}

void Branch::SetLocation(const std::string &Location) {
    this->m_Location = Location;
}

Restaurant::Restaurant(const std::string &Name) : m_Name(Name) {
}

void Restaurant::AddBranch(const std::shared_ptr<Branch> &branch) {
    if (branch == nullptr) {
        throw std::invalid_argument("Branch cannot be null");
    }
    this->m_Branches.emplace_back(branch);
}

void Restaurant::RemoveBranch(const std::shared_ptr<Branch> &branch) {
    if (branch == nullptr) {
        throw std::invalid_argument("Branch cannot be null");
    }
    this->RemoveBranch(branch->GetID());
}

void Restaurant::RemoveBranch(const int BranchID) {
    for (const auto &BranchPtr: this->m_Branches) {
        if (BranchPtr->GetID() == BranchID) {
            std::erase(this->m_Branches, BranchPtr);
            return;
        }
    }
    throw std::invalid_argument("Branch with ID " + std::to_string(BranchID) + " not found");
}

std::string Restaurant::GetName() const {
    return this->m_Name;
}

void Restaurant::SetName(const std::string &Name) {
    this->m_Name = Name;
}

std::vector<std::shared_ptr<Branch> > Restaurant::GetBranches() const {
    return this->m_Branches;
}
