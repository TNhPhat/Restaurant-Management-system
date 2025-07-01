#pragma once
#include <string>
#include <Logger.hpp>
#include <memory>
#include <stdexcept>

class Resource
{
private:
    const int m_ResourceID;
    std::string m_Name;
    int m_Quantity;
    float m_Price;

public:
    // Resource();
    Resource(const int ID);
    ~Resource() = default;
    void SetName(std::string Name);
    void SetQuantity(int Quantity);
    void SetPrice(float Price);
    void Add(int q);
    bool Use(int q);
    std::string GetName() const;
    int GetID() const;
    int GetQuantity() const;
    float GetPrice() const;
};
