#pragma once
#include <vector>
#include <string>
#include <memory>

#include "FileHandle.hpp"

struct MealIngredient {
    std::string Name;
    int Quantity;
};

class MenuAddon {
private:
    const int m_AddonID;
    std::string m_Name;
    double m_Price;
    std::vector<MealIngredient> m_Ingredients;

public:
    MenuAddon(const std::string &Name, double Price, const std::vector<MealIngredient> &Ingredients);

    MenuAddon(const int ID, const std::string &Name, double Price, const std::vector<MealIngredient> &Ingredients);

    int GetID() const;

    std::string GetName() const;

    double GetPrice() const;

    std::vector<MealIngredient> GetIngredients() const;

    void SetName(const std::string &Name);

    void SetPrice(double Price);

    void AddIngredient(const MealIngredient &Ingredient);

    void RemoveIngredient(const std::string &IngredientName);
};

class MenuItem {
private:
    const int m_MenuItemID;
    double m_Price;
    std::string m_Title;
    std::string m_Description;
    std::vector<MealIngredient> m_Ingredients;
    std::vector<std::shared_ptr<MenuAddon> > m_AvailableAddons;

public:
    MenuItem(const std::string &Title, const std::string &Description, double Price);

    MenuItem(const int ID, const std::string &Title, const std::string &Description, double Price);

    ~MenuItem() = default;

    int GetID() const;

    std::string GetTitle() const;

    std::string GetDescription() const;

    double GetPrice() const;

    std::vector<MealIngredient> GetIngredients() const;

    std::vector<std::shared_ptr<MenuAddon> > GetAvailableAddons() const;

    void SetTitle(const std::string &Title);

    void SetDescription(const std::string &Description);

    void SetPrice(double Price);

    void AddIngredient(std::string Ingredient, int Quantity);

    void RemoveIngredient(const std::string &Ingredient);

    void AddAddon(const std::shared_ptr<MenuAddon> &Addon);

    void RemoveAddon(const std::shared_ptr<MenuAddon> &Addon);
};

class MenuSection {
private:
    const int m_MenuSectionID;
    std::string m_Title;
    std::string m_Description;
    std::vector<std::shared_ptr<MenuItem> > m_MenuItems;

public:
    MenuSection(const std::string &Title, const std::string &Description);

    MenuSection(const int ID, const std::string &Title, const std::string &Description);

    ~MenuSection() = default;

    int GetID() const;

    std::string GetTitle() const;

    std::string GetDescription() const;

    void SetTitle(const std::string &Title);

    void SetDescription(const std::string &Description);

    void AddMenuItem(std::shared_ptr<MenuItem> Item);

    template<typename... Args>
    void AddMenuItem(Args &&... args);

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;
};

template<typename... Args>
void MenuSection::AddMenuItem(Args &&... args) {
    AddMenuItem(std::make_shared<const MenuItem>(std::forward<Args>(args)...));
}

class Menu {
private:
    const int m_MenuID;
    std::string m_Name;
    std::string m_Description;
    std::vector<std::shared_ptr<MenuSection> > m_Sections;

public:
    Menu(const std::string &Name, const std::string &Description);

    Menu(const int ID, const std::string &Name, const std::string &Description);

    ~Menu() = default;

    void AddSection(const MenuSection &Section);

    void AddSection(std::shared_ptr<MenuSection> Section);

    template<typename... Args>
    void AddSection(Args &&... args);

    int GetID() const;

    std::string GetName() const;

    std::string GetDescription() const;

    std::vector<std::shared_ptr<MenuSection> > GetSections() const;

    void SetName(const std::string &Name);

    void SetDescription(const std::string &Description);
};

template<typename... Args>
void Menu::AddSection(Args &&... args) {
    AddSection(std::make_shared<MenuSection>(std::forward<Args>(args)...));
}
