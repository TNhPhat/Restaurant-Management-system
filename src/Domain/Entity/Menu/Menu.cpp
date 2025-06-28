#include "Menu.hpp"
#include "../../../Core/Utils/IDManager.hpp"

MenuItem::MenuItem(std::string Title, std::string Description, double Price) : m_Title(Title), m_Description(Description), m_Price(Price), m_MenuItemID(IDManager::GetInstance().GetNextID("MenuItem"))
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

std::vector<MealIngredient> MenuItem::GetIngredients() const
{
    return this->m_Ingredients;
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

void MenuItem::AddIngredient(std::string Ingredient, int Quantity)
{
    bool found = false;
    for (auto &iter : this->m_Ingredients)
    {
        if (iter.Name == Ingredient)
        {
            found = true;
            iter.Quantity += Quantity;
            break;
        }
    }
    if (found == false)
    {
        this->m_Ingredients.emplace_back(Ingredient, Quantity);
    }
}

void MenuItem::RemoveIngredient(std::string Ingredient)
{
    for (size_t index = 0; index < this->m_Ingredients.size(); index++)
    {
        if (this->m_Ingredients[index].Name == Ingredient)
        {
            this->m_Ingredients.erase(this->m_Ingredients.begin() + index);
        }
    }
}

MenuSection::MenuSection(std::string Title, std::string Description) : m_Title(Title), m_Description(Description), m_MenuSectionID(IDManager::GetInstance().GetNextID("MenuSection"))
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

std::vector<std::shared_ptr<const MenuItem>> MenuSection::GetMenuItems() const
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

void MenuSection::AddMenuItem(std::shared_ptr<const MenuItem> Item)
{
    m_MenuItems.emplace_back(Item);
}

template <typename... Args>
void MenuSection::AddMenuItem(Args &&...args)
{
    AddMenuItem(std::make_shared<const MenuItem>(std::forward<Args>(args)...));
}

void MenuSection::GetMenuSection() const
{
    return;
}

Menu::Menu(std::string Title, std::string Description) : m_Title(Title), m_Description(Description), m_MenuID(IDManager::GetInstance().GetNextID("Menu"))
{
}

void Menu::AddSection(std::shared_ptr<const MenuSection> Section)
{
    this->m_Sections.emplace_back(Section);
}

template <typename... Args>
void Menu::AddSection(Args &&...args)
{
    AddSection(std::make_shared<const MenuSection>(std::forward<Args>(args)...));
}

void Menu::GetMenu() const
{
    return;
}

int Menu::GetID() const
{
    return this->m_MenuID;
}

const std::vector<std::shared_ptr<const MenuSection>> Menu::GetSections() const
{
    return this->m_Sections;
}