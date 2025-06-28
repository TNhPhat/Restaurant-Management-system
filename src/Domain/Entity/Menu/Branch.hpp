#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Menu.hpp"

class Branch
{
private:
    const int m_BranchID;
    std::string m_Name;
    std::string m_Location;
    const Menu &m_Menu;

public:
    Branch(std::string Name, std::string Location, const Menu &menu);
    int GetID() const;
    std::string GetName() const;
    std::string GetLocation() const;
    void SetName(std::string Name);
    void SetLocation(std::string Location);
};

class Restaurant
{
private:
    std::string m_Name;
    std::vector<std::shared_ptr<const Branch>> m_Branches;

public:
    Restaurant(std::string Name);
    void AddBranch(std::shared_ptr<const Branch> branch);
    template <typename... Args>
    void AddBranch(Args &&...args);
    const std::vector<std::shared_ptr<const Branch>> GetBranches() const;
};