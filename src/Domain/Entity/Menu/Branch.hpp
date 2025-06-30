#pragma once
#include <vector>
#include <string>
#include <memory>

#include "FileHandle.hpp"
#include "Menu.hpp"

class Branch
{
private:
    const int m_BranchID;
    std::string m_Name;
    std::string m_Location;
    std::shared_ptr<Menu> m_Menu;

public:
    Branch(const std::string &Name, const std::string &Location, const std::shared_ptr<Menu> &Menu);

    Branch(const int ID, const std::string &Name, const std::string &Location, const std::shared_ptr<Menu> &menu);

    int GetID() const;

    std::string GetName() const;

    std::string GetLocation() const;

    const std::shared_ptr<Menu> &GetMenu() const;

    void SetName(const std::string &Name);

    void SetLocation(const std::string &Location);
};

class Restaurant
{
private:
    std::string m_Name;
    std::vector<std::shared_ptr<Branch>> m_Branches;

public:
    Restaurant(const std::string &Name);

    void AddBranch(std::shared_ptr<Branch> branch);

    template <typename... Args>
    void AddBranch(Args &&...args);

    void RemoveBranch(const std::shared_ptr<Branch> &branch);

    void RemoveBranch(int BranchID);

    std::string GetName() const;

    void SetName(const std::string &Name);

    std::vector<std::shared_ptr<Branch>> GetBranches() const;
};

template <typename... Args>
void Restaurant::AddBranch(Args &&...args)
{
    AddBranch(std::make_shared<Branch>(std::forward<Args>(args)...));
}