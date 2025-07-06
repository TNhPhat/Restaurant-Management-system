#include "Storage.hpp"


Storage::Storage(const int ID):m_StorageID(ID){}
Storage::~Storage()
{
    m_Resources.clear();
}

const int Storage::GetID() const{
    return this->m_StorageID;
}
const std::vector<std::shared_ptr<Resource>> Storage::GetResources() const{
    return m_Resources;
}
Resource *Storage::SearchByID(const int ID)
{
    for (const auto& tmp : this->m_Resources){
        if (tmp && tmp->GetID() == ID) {
            return tmp.get();
        }
    }
    return nullptr;
}

bool Storage::AddResource(int ID, std::string Name, int Quantity, float Price)
{
    if (SearchByID(ID) != nullptr)
    {
        // Log error
        throw std::invalid_argument("Resource ID must be unique");
    }
    auto newResource = std::make_shared<Resource>(ID);
    newResource->SetName(Name);
    newResource->SetQuantity(Quantity);
    newResource->SetPrice(Price);
    this->m_Resources.push_back(newResource);
}

bool Storage::Use(int ID, int q)
{
    Resource *r = SearchByID(ID);
    if (r == nullptr)
    {
        // Log error
        throw std::invalid_argument("Can not find resource");
    }
    r->Use(q);
    return true;
}

bool Storage::Add(int ID, int q)
{
    Resource *r = SearchByID(ID);
    if (r == nullptr)
    {
        // Log error
        throw std::invalid_argument("Can not find resource");
    }
    r->Add(q);
    return true;
}
