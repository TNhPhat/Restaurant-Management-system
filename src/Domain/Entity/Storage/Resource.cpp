#include "Resource.hpp"

Resource::Resource(const int ID) : m_Name(""), m_Quantity(0), m_Price(0), m_ResourceID(ID) {}

void Resource::SetName(std::string Name)
{
    this->m_Name = Name;
}

void Resource::SetQuantity(int Quantity)
{
    this->m_Quantity = Quantity;
}

void Resource::SetPrice(float Price)
{
    this->m_Price = Price;
}

void Resource::Add(int q)
{
    this->m_Quantity += q;
}

bool Resource::Use(int q)
{
    if (q > this->m_Quantity)
    {
        // Log error
        throw std::invalid_argument("");
    }

    this->m_Quantity -= q;
    return true;
}
std::string Resource::GetName() const
{
    return this->m_Name;
}
int Resource::GetID() const
{
    return this->m_ResourceID;
}
int Resource::GetQuantity() const
{
    return m_Quantity;
}
float Resource::GetPrice() const
{
    return m_Price;
}