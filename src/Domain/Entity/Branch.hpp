#pragma once
#include <vector>
#include <string>
#include "Menu.hpp"

class Branch
{
private:
    const int m_BranchID;
    static int m_BranchCount;
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
    std::vector<const Branch &> m_Branches;

public:
    Restaurant(std::string Name);
    void AddBranch(const Branch &branch);
    const std::vector<const Branch &> GetBranches() const;
};