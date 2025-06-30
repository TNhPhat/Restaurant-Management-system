#include "Menu.hpp"
#include "../../../Core/Utils/IDManager.hpp"

MenuAddon::MenuAddon(const std::string &Name, const double Price,
                     const std::vector<MealIngredient> &Ingredients) : MenuAddon(
    IDManager::GetInstance().GetNextID("MenuAddon"), Name, Price, Ingredients) {
}

MenuAddon::MenuAddon(const int ID, const std::string &Name, const double Price,
                     const std::vector<MealIngredient> &Ingredients) : m_AddonID(ID), m_Name(Name), m_Price(Price),
                                                                       m_Ingredients(Ingredients) {
}

int MenuAddon::GetID() const {
    return this->m_AddonID;
}

std::string MenuAddon::GetName() const {
    return this->m_Name;
}

double MenuAddon::GetPrice() const {
    return this->m_Price;
}

std::vector<MealIngredient> MenuAddon::GetIngredients() const {
    return this->m_Ingredients;
}

void MenuAddon::SetName(const std::string &Name) {
    this->m_Name = Name;
}

void MenuAddon::SetPrice(double Price) {
    this->m_Price = Price;
}

void MenuAddon::AddIngredient(const MealIngredient &Ingredient) {
    this->m_Ingredients.emplace_back(Ingredient);
}

void MenuAddon::RemoveIngredient(const std::string &IngredientName) {
    for (auto it = this->m_Ingredients.begin(); it != this->m_Ingredients.end(); ++it) {
        if (it->Name == IngredientName) {
            this->m_Ingredients.erase(it);
            return;
        }
    }
}

MenuItem::MenuItem(const std::string &Title, const std::string &Description, const double Price) : MenuItem(
    IDManager::GetInstance().GetNextID("MenuItem"), Title, Description, Price) {
}

MenuItem::MenuItem(const int ID, const std::string &Title, const std::string &Description,
                   const double Price) : m_MenuItemID(ID), m_Price(Price),
                                         m_Title(Title), m_Description(Description) {
    if (ID <= 0) {
        throw std::invalid_argument("MenuItem ID must be greater than zero");
    }
    if (Title.empty()) {
        throw std::invalid_argument("MenuItem title cannot be empty");
    }
    if (Description.empty()) {
        throw std::invalid_argument("MenuItem description cannot be empty");
    }
}

std::string MenuItem::GetTitle() const {
    return this->m_Title;
}

std::string MenuItem::GetDescription() const {
    return this->m_Description;
}

double MenuItem::GetPrice() const {
    return this->m_Price;
}

int MenuItem::GetID() const {
    return this->m_MenuItemID;
}

std::vector<MealIngredient> MenuItem::GetIngredients() const {
    return this->m_Ingredients;
}

std::vector<std::shared_ptr<MenuAddon> > MenuItem::GetAvailableAddons() const {
    return this->m_AvailableAddons;
}

void MenuItem::SetTitle(const std::string &Title) {
    m_Title = Title;
}

void MenuItem::SetDescription(const std::string &Description) {
    m_Description = Description;
}

void MenuItem::SetPrice(const double Price) {
    m_Price = Price;
}

void MenuItem::AddIngredient(std::string Ingredient, int Quantity) {
    bool found = false;
    for (auto &iter: this->m_Ingredients) {
        if (iter.Name == Ingredient) {
            found = true;
            iter.Quantity += Quantity;
            break;
        }
    }
    if (found == false) {
        this->m_Ingredients.emplace_back(Ingredient, Quantity);
    }
}

void MenuItem::RemoveIngredient(const std::string &Ingredient) {
    for (size_t index = 0; index < this->m_Ingredients.size(); index++) {
        if (this->m_Ingredients[index].Name == Ingredient) {
            this->m_Ingredients.erase(this->m_Ingredients.begin() + index);
        }
    }
}

void MenuItem::AddAddon(const std::shared_ptr<MenuAddon> &Addon) {
    if (Addon == nullptr) {
        throw std::invalid_argument("MenuAddon cannot be null");
    }
    this->m_AvailableAddons.emplace_back(Addon);
}

void MenuItem::RemoveAddon(const std::shared_ptr<MenuAddon> &Addon) {
    if (Addon == nullptr) {
        throw std::invalid_argument("MenuAddon cannot be null");
    }
    for (const auto &existingAddon: this->m_AvailableAddons) {
        if (existingAddon->GetID() == Addon->GetID()) {
            std::erase(this->m_AvailableAddons, existingAddon);
            return;
        }
    }
    throw std::invalid_argument("MenuAddon with ID " + std::to_string(Addon->GetID()) + " not found");
}

MenuSection::MenuSection(const std::string &Title, const std::string &Description) : MenuSection(
    IDManager::GetInstance().GetNextID("MenuSection"), Title, Description) {
}

MenuSection::MenuSection(const int ID, const std::string &Title, const std::string &Description) : m_MenuSectionID(ID),
    m_Title(Title),
    m_Description(Description) {
    if (ID <= 0) {
        throw std::invalid_argument("MenuSection ID must be greater than zero");
    }
    if (Title.empty()) {
        throw std::invalid_argument("MenuSection title cannot be empty");
    }
    if (Description.empty()) {
        throw std::invalid_argument("MenuSection description cannot be empty");
    }
}

std::string MenuSection::GetTitle() const {
    return this->m_Title;
}

std::string MenuSection::GetDescription() const {
    return this->m_Description;
}

int MenuSection::GetID() const {
    return this->m_MenuSectionID;
}

std::vector<std::shared_ptr<MenuItem> > MenuSection::GetMenuItems() const {
    return this->m_MenuItems;
}

void MenuSection::SetTitle(const std::string &Title) {
    m_Title = Title;
}

void MenuSection::SetDescription(const std::string &Description) {
    m_Description = Description;
}

void MenuSection::AddMenuItem(std::shared_ptr<MenuItem> Item) {
    if (Item == nullptr) {
        throw std::invalid_argument("MenuItem cannot be null");
    }
    m_MenuItems.emplace_back(Item);
}

Menu::Menu(const std::string &Name, const std::string &Description) : Menu(
    IDManager::GetInstance().GetNextID("Menu"), Name, Description) {
}

Menu::Menu(const int ID, const std::string &Name, const std::string &Description) : m_MenuID(ID), m_Name(Name),
    m_Description(Description) {
}

void Menu::AddSection(std::shared_ptr<MenuSection> Section) {
    if (Section == nullptr) {
        throw std::invalid_argument("MenuSection cannot be null");
    }
    this->m_Sections.emplace_back(Section);
}

int Menu::GetID() const {
    return this->m_MenuID;
}

std::vector<std::shared_ptr<MenuSection> > Menu::GetSections() const {
    return this->m_Sections;
}

void Menu::AddSection(const MenuSection &Section) {
    this->m_Sections.emplace_back(std::make_shared<MenuSection>(Section));
}

std::string Menu::GetName() const {
    return this->m_Name;
}

std::string Menu::GetDescription() const {
    return this->m_Description;
}

void Menu::SetName(const std::string &Name) {
    this->m_Name = Name;
}

void Menu::SetDescription(const std::string &Description) {
    this->m_Description = Description;
}
