#include "Meal.hpp"
#include "IDManager.hpp"
#include "FileHandle/FileHandle.hpp"

MealItem::MealItem(const int ID, const MenuItem &Item, int Quantity, const std::string &Note) : m_MealItemID(ID),
                                                                                                m_MenuItem(Item), m_Quantity(Quantity), m_Note(Note)
{
    if (ID <= 0)
    {
        throw std::invalid_argument("Meal Item ID must be greater than zero");
    }
    if (Quantity <= 0)
    {
        throw std::invalid_argument("Quantity must be greater than zero");
    }
}

MealItem::MealItem(const MenuItem &Item, const int Quantity) : MealItem(IDManager::GetInstance().GetNextID("MealItem"),
                                                                        Item, Quantity, "")
{
}

MealItem::MealItem(const MenuItem &Item, const int Quantity, const std::string &Note) : MealItem(
                                                                                            IDManager::GetInstance().GetNextID("MealItem"), Item, Quantity, Note)
{
}

MealItem::MealItem(const MealItem &Origin) : m_MealItemID(Origin.m_MealItemID), m_MenuItem(Origin.m_MenuItem),
                                             m_Quantity(Origin.m_Quantity), m_Addons(Origin.m_Addons),
                                             m_Note(Origin.m_Note)
{
}

double MealItem::GetPrice() const
{
    return this->m_Quantity * this->m_MenuItem.GetPrice();
}

int MealItem::GetID() const
{
    return this->m_MealItemID;
}

int MealItem::GetQuantity() const
{
    return this->m_Quantity;
}

std::vector<std::shared_ptr<MealAddon>> MealItem::GetAddons() const
{
    return this->m_Addons;
}

std::string MealItem::GetNote() const
{
    return this->m_Note;
}

const MenuItem &MealItem::GetMenuItem() const
{
    return this->m_MenuItem;
}

bool MealItem::ContainsAddon(const int AddonID) const
{
    for (const auto &Addon : this->m_Addons)
    {
        if (Addon->GetAddon()->GetID() == AddonID)
        {
            return true;
        }
    }
    return false;
}

void MealItem::SetQuantity(const int Quantity)
{
    this->m_Quantity = Quantity;
}

std::vector<MealIngredient> MealItem::GetResources() const
{
    std::unordered_map<std::string, int> ingredients;
    for (const auto &[Name, Quantity] : this->m_MenuItem.GetIngredients())
    {
        ingredients[Name] += Quantity * this->m_Quantity;
    }
    for (const auto &AddonPtr : this->m_Addons)
    {
        for (auto addonIngredients = AddonPtr->GetIngredients(); const auto &[Name, Quantity] : addonIngredients)
        {
            ingredients[Name] += Quantity * AddonPtr->GetQuantity();
        }
    }
    std::vector<MealIngredient> result(ingredients.begin(), ingredients.end());
    return result;
}

void MealItem::SetNote(const std::string &Note)
{
    this->m_Note = Note;
}

void MealItem::AddAddon(const std::shared_ptr<MenuAddon> &Addon, unsigned int Quantity)
{
    if (Addon == nullptr)
    {
        throw std::invalid_argument("Addon cannot be null");
    }
    for (const auto &existingAddon : this->m_Addons)
    {
        if (existingAddon->GetAddon()->GetID() == Addon->GetID())
        {
            existingAddon->AddQuantity(Quantity);
            return;
        }
    }
    this->m_Addons.emplace_back(std::make_shared<MealAddon>(Addon, Quantity));
}

void MealItem::RemoveAddon(const std::string &AddonName, const unsigned int Quantity)
{
    for (const auto &existingAddon : this->m_Addons)
    {
        if (existingAddon->GetAddon()->GetName() == AddonName)
        {
            existingAddon->RemoveQuantity(Quantity);
            if (existingAddon->GetQuantity() <= 0)
            {
                std::erase(this->m_Addons, existingAddon);
            }
            return;
        }
    }
    throw std::invalid_argument("Addon not found: " + AddonName);
}

void MealItem::RemoveAddon(const int AddonID, const unsigned int Quantity)
{
    for (const auto &existingAddon : this->m_Addons)
    {
        if (existingAddon->GetAddon()->GetID() == AddonID)
        {
            existingAddon->RemoveQuantity(Quantity);
            if (existingAddon->GetQuantity() <= 0)
            {
                std::erase(this->m_Addons, existingAddon);
            }
            return;
        }
    }
    throw std::invalid_argument("Addon not found with ID: " + std::to_string(AddonID));
}

Meal::Meal() : m_MealID(IDManager::GetInstance().GetNextID("Meal"))
{
}

Meal::Meal(const int ID) : m_MealID(ID)
{
    if (ID <= 0)
    {
        throw std::invalid_argument("Meal ID must be greater than zero");
    }
}

void Meal::AddItem(const std::shared_ptr<MealItem> &Item)
{
    if (Item == nullptr)
    {
        throw std::invalid_argument("MealItem cannot be null");
    }
    this->m_MealItems.emplace_back(Item);
}

int Meal::GetID() const
{
    return this->m_MealID;
}

double Meal::GetPrice() const
{
    double total = 0;
    for (const auto &m_MealItem : m_MealItems)
    {
        total += m_MealItem->GetPrice();
    }
    return total;
}

std::vector<std::shared_ptr<MealItem>> Meal::GetMealItems() const
{
    return this->m_MealItems;
}

std::vector<MealIngredient> Meal::GetResources() const
{
    std::unordered_map<std::string, int> ingredients;
    for (const auto &Item : this->m_MealItems)
    {
        for (const auto &[Name, Quantity] : Item->GetResources())
        {
            ingredients[Name] += Quantity;
        }
    }
    std::vector<MealIngredient> result(ingredients.begin(), ingredients.end());
    return result;
}

bool Meal::ContainsItem(const int ItemID) const
{
    for (const auto &Item : this->m_MealItems)
    {
        if (Item->GetID() == ItemID)
        {
            return true;
        }
    }
    return false;
}

void Meal::RemoveItem(const int ItemID)
{
    for (const auto &Item : this->m_MealItems)
    {
        if (Item->GetID() == ItemID)
        {
            std::erase(this->m_MealItems, Item);
            return;
        }
    }
    LOG_ERROR("MealItem with ID {} not found in Meal", ItemID);
}
