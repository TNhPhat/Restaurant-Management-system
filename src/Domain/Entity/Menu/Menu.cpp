#include "Menu.hpp"

#include <utility>
#include "../../../Core/Utils/IDManager.hpp"

MenuAddon::MenuAddon(const std::string &Name, const double Price,
                     const std::vector<MealIngredient> &Ingredients) : MenuAddon(IDManager::GetInstance().GetNextID("MenuAddon"), Name, Price, Ingredients)
{
}

MenuAddon::MenuAddon(const int ID, std::string Name, const double Price,
                     const std::vector<MealIngredient> &Ingredients) : m_AddonID(ID), m_Name(std::move(Name)),
                                                                       m_Price(Price)
{
    for (const auto it : Ingredients)
    {
        this->m_Ingredients[it.Name] = it.Quantity;
    }
}

int MenuAddon::GetID() const
{
    return this->m_AddonID;
}

std::string MenuAddon::GetName() const
{
    return this->m_Name;
}

double MenuAddon::GetPrice() const
{
    return this->m_Price;
}

std::vector<MealIngredient> MenuAddon::GetIngredients() const
{
    return std::vector<MealIngredient>(this->m_Ingredients.begin(), this->m_Ingredients.end());
}

const std::map<std::string, int> &MenuAddon::GetIngredientsMap() const
{
    return this->m_Ingredients;
}

void MenuAddon::SetName(const std::string &Name)
{
    this->m_Name = Name;
}

void MenuAddon::SetPrice(const double Price)
{
    this->m_Price = Price;
}

void MenuAddon::AddIngredient(const MealIngredient &Ingredient)
{
    this->m_Ingredients[Ingredient.Name] = Ingredient.Quantity;
}

void MenuAddon::RemoveIngredient(const std::string &IngredientName)
{
    for (auto it = this->m_Ingredients.begin(); it != this->m_Ingredients.end(); ++it)
    {
        if (it->first == IngredientName)
        {
            this->m_Ingredients.erase(it);
            return;
        }
    }
}

MenuItem::MenuItem(const std::string &Title, const std::string &Description, const double Price) : MenuItem(
                                                                                                       IDManager::GetInstance().GetNextID("MenuItem"), Title, Description, Price)
{
}

MenuItem::MenuItem(const int ID, const std::string &Title, const std::string &Description,
                   const double Price) : m_MenuItemID(ID), m_Price(Price),
                                         m_Title(Title), m_Description(Description)
{
    if (ID <= 0)
    {
        throw std::invalid_argument("MenuItem ID must be greater than zero");
    }
    if (Title.empty())
    {
        throw std::invalid_argument("MenuItem title cannot be empty");
    }
    if (Description.empty())
    {
        throw std::invalid_argument("MenuItem description cannot be empty");
    }
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
    return std::vector<MealIngredient>(this->m_Ingredients.begin(), this->m_Ingredients.end());
}

std::vector<std::shared_ptr<MenuAddon>> MenuItem::GetAvailableAddons() const
{
    return this->m_AvailableAddons;
}

bool MenuItem::ContainsAddon(const int AddonID) const
{
    for (const auto &Addon : this->m_AvailableAddons)
    {
        if (Addon->GetID() == AddonID)
        {
            return true;
        }
    }
    return false;
}

void MenuItem::SetTitle(const std::string &Title)
{
    m_Title = Title;
}

void MenuItem::SetDescription(const std::string &Description)
{
    m_Description = Description;
}

void MenuItem::SetPrice(const double Price)
{
    m_Price = Price;
}

void MenuItem::AddIngredient(const std::string &Ingredient, const int Quantity)
{
    this->m_Ingredients[Ingredient] = Quantity;
}

void MenuItem::RemoveIngredient(const std::string &Ingredient)
{
    for (auto it = this->m_Ingredients.begin(); it != this->m_Ingredients.end(); ++it)
    {
        if (it->first == Ingredient)
        {
            this->m_Ingredients.erase(it);
            return;
        }
    }
    throw std::invalid_argument("Ingredient " + Ingredient + " not found in MenuItem");
}

void MenuItem::AddAddon(const std::shared_ptr<MenuAddon> &Addon)
{
    if (Addon == nullptr)
    {
        throw std::invalid_argument("MenuAddon cannot be null");
    }
    this->m_AvailableAddons.emplace_back(Addon);
}

void MenuItem::RemoveAddon(const std::shared_ptr<MenuAddon> &Addon)
{
    if (Addon == nullptr)
    {
        throw std::invalid_argument("MenuAddon cannot be null");
    }
    for (const auto &existingAddon : this->m_AvailableAddons)
    {
        if (existingAddon->GetID() == Addon->GetID())
        {
            std::erase(this->m_AvailableAddons, existingAddon);
            return;
        }
    }
    throw std::invalid_argument("MenuAddon with ID " + std::to_string(Addon->GetID()) + " not found");
}

const std::map<std::string, int> &MenuItem::GetIngredientsMap() const
{
    return this->m_Ingredients;
}

MenuSection::MenuSection(const std::string &Title, const std::string &Description) : MenuSection(
                                                                                         IDManager::GetInstance().GetNextID("MenuSection"), Title, Description)
{
}

MenuSection::MenuSection(const int ID, const std::string &Title, const std::string &Description) : m_MenuSectionID(ID),
                                                                                                   m_Title(Title),
                                                                                                   m_Description(Description)
{
    if (ID <= 0)
    {
        throw std::invalid_argument("MenuSection ID must be greater than zero");
    }
    if (Title.empty())
    {
        throw std::invalid_argument("MenuSection title cannot be empty");
    }
    if (Description.empty())
    {
        throw std::invalid_argument("MenuSection description cannot be empty");
    }
}

std::string MenuSection::GetTitle() const
{
    return this->m_Title;
}

std::string MenuSection::GetDescription() const
{
    return this->m_Description;
}

bool MenuSection::ContainsItem(const int ItemID) const
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetID() == ItemID)
        {
            return true;
        }
    }
    return false;
}

int MenuSection::GetID() const
{
    return this->m_MenuSectionID;
}

std::vector<std::shared_ptr<MenuItem>> MenuSection::GetMenuItems() const
{
    return this->m_MenuItems;
}

void MenuSection::SetTitle(const std::string &Title)
{
    m_Title = Title;
}

void MenuSection::SetDescription(const std::string &Description)
{
    m_Description = Description;
}

std::shared_ptr<MenuItem> MenuSection::AddMenuItem(const std::shared_ptr<MenuItem> &Item)
{
    if (Item == nullptr)
    {
        throw std::invalid_argument("MenuItem cannot be null");
    }
    m_MenuItems.emplace_back(Item);
    return Item;
}

void MenuSection::RemoveItem(const int ItemID)
{
    for (const auto &Item : this->m_MenuItems)
    {
        if (Item->GetID() == ItemID)
        {
            std::erase(this->m_MenuItems, Item);
            return;
        }
    }
    throw std::invalid_argument("MenuItem with ID " + std::to_string(ItemID) + " not found in MenuSection");
}

Menu::Menu(const std::string &Name, const std::string &Description) : Menu(
                                                                          IDManager::GetInstance().GetNextID("Menu"), Name, Description)
{
}

Menu::Menu(const int ID, std::string Name, std::string Description) : m_MenuID(ID), m_Name(std::move(Name)),
                                                                      m_Description(std::move(Description))
{
}

std::shared_ptr<MenuSection> Menu::AddSection(const std::shared_ptr<MenuSection> &Section)
{
    if (Section == nullptr)
    {
        throw std::invalid_argument("MenuSection cannot be null");
    }
    this->m_Sections.emplace_back(Section);
    return this->m_Sections.back();
}

int Menu::GetID() const
{
    return this->m_MenuID;
}

std::vector<std::shared_ptr<MenuSection>> Menu::GetSections() const
{
    return this->m_Sections;
}

std::string Menu::GetName() const
{
    return this->m_Name;
}

std::string Menu::GetDescription() const
{
    return this->m_Description;
}

bool Menu::ContainsSection(const int SectionID) const
{
    for (const auto &Section : this->m_Sections)
    {
        if (Section->GetID() == SectionID)
        {
            return true;
        }
    }
    return false;
}

std::shared_ptr<MenuSection> Menu::GetSectionByID(int SectionID) const
{
    for (const auto &Section : this->m_Sections)
    {
        if (Section->GetID() == SectionID)
        {
            return Section;
        }
    }
    return nullptr;
}

void Menu::SetName(const std::string &Name)
{
    this->m_Name = Name;
}

void Menu::SetDescription(const std::string &Description)
{
    this->m_Description = Description;
}
