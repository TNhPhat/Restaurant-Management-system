#pragma once
#include <string>

class Resource
{
private:
    std::string m_Name;
    int m_Quantity;
    float m_Price;

public:
    Resource();
    ~Resource() = default;
    void SetName(std::string Name);
    void SetQuantity(int Quantity);
    void SetPrice(float Price);
    void Add(int q);
    bool Use(int q);
    std::string GetName() const;
};


