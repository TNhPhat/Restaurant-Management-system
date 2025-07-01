#pragma once
#include "Resource.hpp"
#include <vector>

class Storage
{
private:
    std::vector<std::shared_ptr<Resource>> m_Resources;
    Storage() = default;
public:
    ~Storage();
    Resource *SearchByID(const int ID);
    bool AddResource(int ID, std::string Name, int Quantity, float Price);
    bool Use(int ID, int q);
    bool Add(int ID, int q);
};


