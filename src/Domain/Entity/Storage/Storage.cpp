#include "Storage.hpp"
#include <vector>
#include <stdexcept>

Storage::~Storage()
{
    m_Resources.clear();
}

Resource *Storage::SearchByName(std::string Name){
    for (auto r : m_Resources) {
        if (r.GetName() == Name) {
            return &r;
        }
    }
    return nullptr;
}

bool Storage::AddResource(std::string Name, int Quantity, float Price)
{
    if (SearchByName(Name) != nullptr){
        //Log error

        return false;
    }
    Resource newResource;
    newResource.SetName(Name);
    newResource.SetQuantity(Quantity);
    newResource.SetPrice(Price);
    this->m_Resources.push_back(newResource);
}

bool Storage::Use(std::string Name, int q)
{
    Resource * r = SearchByName(Name);
    if (r == nullptr || !r->Use(q)){
        //Log error

        return false;
    }

    return true;
}

bool Storage::Add(std::string Name, int q)
{
    Resource * r = SearchByName(Name);
    if (r == nullptr){
        //Log error

        return false;
    }
    r->Add(q);
    return true;
}
