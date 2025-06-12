#pragma once
#include "Resource.hpp"
#include <vector>

class Storage
{
private:
    std::vector<Resource> m_Resources;
    Storage() = default;
public:
    ~Storage();
    Resource *SearchByName(std::string Name);
    bool AddResource(std::string Name, int Quantity, float Price);
    bool Use(std::string Name, int q);
    bool Add(std::string Name, int q);
};


