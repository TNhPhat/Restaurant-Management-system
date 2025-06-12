#pragma once
#include <vector>
#include <string>
class MenuItem
{
private:
    static int m_MenuItemCount;
    const int m_MenuItemID;
    double m_Price;
    std::string m_Title;
    std::string m_Description;

public:
    MenuItem(std::string Title, std::string Description, double Price);
    ~MenuItem() = default;
    int GetID() const;
    std::string GetTitle() const;
    std::string GetDescription() const;
    double GetPrice() const;
    void SetTitle(std::string Title);
    void SetDescription(std::string Description);
    void SetPrice(double Price);
};

class MenuSection
{
private:
    static int m_MenuSectionCount;
    const int m_MenuSectionID;
    std::string m_Title;
    std::string m_Description;
    std::vector<const MenuItem &> m_MenuItems;

public:
    MenuSection(std::string Title, std::string Description);
    ~MenuSection() = default;
    int GetID() const;
    std::string GetTitle() const;
    std::string GetDescription() const;
    void SetTitle(std::string Title);
    void SetDescription(std::string Description);
    void AddMenuItem(const MenuItem &Item);
    const std::vector<const MenuItem &> GetMenuItems() const;
    void GetMenuSection() const;
};

class Menu
{
private:
    const int m_MenuID;
    static int m_MenuCount;
    std::string m_Title;
    std::string m_Description;
    std::vector<const MenuSection &> m_Sections;

public:
    Menu(std::string Title, std::string Description);
    ~Menu() = default;
    void GetMenu() const;
    void AddSection(const MenuSection &Section);
    int GetID() const;
    std::string GetTitle() const;
    std::string GetDescription() const;
    const std::vector<const MenuSection &> GetSections() const;
    void SetTitle(std::string Title);
    void SetDescription(std::string Description);
};