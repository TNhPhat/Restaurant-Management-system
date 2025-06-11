#include "Menu.hpp"

MenuItem::MenuItem(std::string Title, std::string Description, double Price) : m_Title(Title), m_Description(Description), m_Price(Price), m_MenuItemID(m_MenuItemCount++)
{
}

std::string MenuItem::GetTitle() const
{
    return this->m_Title;
}

std::string MenuItem::GetDescription() const
{
    return this->m_Description;
}

double MenuItem::GetPrice() const
{
    return this->m_Price;
}

int MenuItem::GetID() const
{
    return this->m_MenuItemID;
}

void MenuItem::SetTitle(std::string Title)
{
    m_Title = Title;
}

void MenuItem::SetDescription(std::string Description)
{
    m_Description = Description;
}

void MenuItem::SetPrice(double Price)
{
    m_Price = Price;
}

MenuSection::MenuSection(std::string Title, std::string Description) : m_Title(Title), m_Description(Description), m_MenuSectionID(m_MenuSectionCount++)
{
}

std::string MenuSection::GetTitle() const
{
    return this->m_Title;
}

std::string MenuSection::GetDescription() const
{
    return this->m_Description;
}

int MenuSection::GetID() const
{
    return this->m_MenuSectionID;
}

const std::vector<const MenuItem &> MenuSection::GetMenuItems() const
{
    return this->m_MenuItems;
}

void MenuSection::SetTitle(std::string Title)
{
    m_Title = Title;
}

void MenuSection::SetDescription(std::string Description)
{
    m_Description = Description;
}

void MenuSection::AddMenuItem(const MenuItem &Item)
{
    m_MenuItems.emplace_back(Item);
}

void MenuSection::GetMenuSection() const
{
    return;
}

Menu::Menu(std::string Title, std::string Description) : m_Title(Title), m_Description(Description), m_MenuID(m_MenuCount++)
{
}

void Menu::AddSection(const MenuSection &Section)
{
    this->m_Sections.emplace_back(Section);
}

void Menu::GetMenu() const
{
    return;
}

int Menu::GetID() const
{
    return this->m_MenuID;
}

const std::vector<const MenuSection &> Menu::GetSections() const
{
    return this->m_Sections;
}