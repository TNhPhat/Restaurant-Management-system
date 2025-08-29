#pragma once
#include "Resource.hpp"
#include <vector>

class Storage
{
private:
    const int m_StorageID;
    std::vector<std::shared_ptr<Resource>> m_Resources;
public:
    Storage(const int ID);
    ~Storage();
    const int GetID() const;
    std::vector<std::shared_ptr<Resource>> &GetResources() ;
    Resource *SearchByID(const int ID);
    bool AddResource(int ID, std::string Name, int Quantity, float Price);
    bool Use(int ID, int q);
    bool Add(int ID, int q);
    bool Delete(int ID);
};


