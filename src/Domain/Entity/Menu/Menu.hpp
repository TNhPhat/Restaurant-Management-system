#pragma once
#include <vector>
#include <string>
#include <memory>

#include "FileHandle.hpp"

#include "MealIngredients.hpp"

class MenuAddon {
private:
    const int m_AddonID;
    std::string m_Name;
    double m_Price;
    std::map<std::string, int> m_Ingredients;

public:
    MenuAddon(const std::string &Name, double Price, const std::vector<MealIngredient> &Ingredients);

    MenuAddon(const int ID, std::string Name, double Price, const std::vector<MealIngredient> &Ingredients);

    int GetID() const;

    std::string GetName() const;

    double GetPrice() const;

    std::vector<MealIngredient> GetIngredients() const;

    const std::map<std::string, int> &GetIngredientsMap() const;

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
    std::map<std::string, int> m_Ingredients;
    std::vector<std::shared_ptr<MenuAddon> > m_AvailableAddons;
    int m_MealCount;

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

    bool ContainsAddon(const int AddonID) const;

    int GetMealCount() const;

    const std::map<std::string, int> &GetIngredientsMap() const;

    void SetTitle(const std::string &Title);

    void SetDescription(const std::string &Description);

    void SetPrice(double Price);

    void AddIngredient(const std::string &Ingredient, int Quantity);

    void RemoveIngredient(const std::string &Ingredient);

    void AddAddon(const std::shared_ptr<MenuAddon> &Addon);

    void RemoveAddon(const std::shared_ptr<MenuAddon> &Addon);

    void IncreaseCount();

    std::shared_ptr<MenuAddon> GetAddon(const int AddonID) const;
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

    std::vector<std::shared_ptr<MenuItem> > GetMenuItems() const;

    bool ContainsItem(const int ItemID) const;

    void SetTitle(const std::string &Title);

    void SetDescription(const std::string &Description);

    std::shared_ptr<MenuItem> AddMenuItem(const std::shared_ptr<MenuItem> &Item);

    template<typename... Args>
    std::shared_ptr<MenuItem> AddMenuItem(Args &&... args);

    void RemoveItem(const int ItemID);

    std::shared_ptr<MenuItem> GetItemByID(int ItemID);

    std::shared_ptr<MenuItem> GetItemByTitle(const std::string &Title);
};

template<typename... Args>
std::shared_ptr<MenuItem> MenuSection::AddMenuItem(Args &&... args) {
    return AddMenuItem(std::make_shared<MenuItem>(std::forward<Args>(args)...));
}

class Menu {
private:
    const int m_MenuID;
    std::string m_Name;
    std::string m_Description;
    std::vector<std::shared_ptr<MenuSection> > m_Sections;

public:
    Menu(const std::string &Name, const std::string &Description);

    Menu(const int ID, std::string Name, std::string Description);

    ~Menu() = default;

    std::shared_ptr<MenuSection> AddSection(const std::shared_ptr<MenuSection> &Section);

    template<typename... Args>
    std::shared_ptr<MenuSection> AddSection(Args &&... args);

    int GetID() const;

    std::string GetName() const;

    std::string GetDescription() const;

    bool ContainsSection(const int SectionID) const;

    std::shared_ptr<MenuSection> GetSectionByID(int SectionID) const;

    std::vector<std::shared_ptr<MenuSection> > GetSections() const;

    void SetName(const std::string &Name);

    void SetDescription(const std::string &Description);

    void RemoveSection(const int SectionID);
};

template<typename... Args>
std::shared_ptr<MenuSection> Menu::AddSection(Args &&... args) {
    return AddSection(std::make_shared<MenuSection>(std::forward<Args>(args)...));
}
