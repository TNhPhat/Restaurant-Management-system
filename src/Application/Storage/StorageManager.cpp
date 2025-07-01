#include <StorageManager.hpp>

StorageManager::StorageManager() : m_FilePath(""), m_IsLoaded(false)
{
    m_FileHandle = std::make_unique<JsonHandle>();
}
StorageManager::~StorageManager()
{
    SaveStorageToFile();
    this->m_Storages.clear();
}

void StorageManager::SetFilePath(const std::string FilePath)
{
    if (this->m_FilePath == "")
    {
        this->m_FilePath = FilePath;
    }
    else
    {
        throw std::invalid_argument("File path already existed");
    }
}
bool StorageManager::LoadStorageFromFile()
{
    if (this->IsLoaded())
    {
        // log error
        throw std::invalid_argument("File is loading...");
    }

    try
    {
        this->m_FileHandle->LoadFile(this->m_FilePath);
        json data = this->m_FileHandle->GetDaTa();

        for (const auto &storageJson : data)
        {
            int StorageID = storageJson["StorageID"];

            auto storage = std::make_shared<Storage>(StorageID);
            for (const auto &resourceJson : storageJson["Resources"])
            {
                const int resourceID = resourceJson["ResourceID"];
                const std::string resourceName = resourceJson["ResourceName"];
                const int resourceQuantity = resourceJson["ResourceQuantity"];
                const float resourcePrice = resourceJson["ResourcePrie"];

                storage->AddResource(resourceID, resourceName, resourceQuantity, resourcePrice);
            }

            this->m_Storages.push_back(storage);
        }

        // Log something
        this->m_IsLoaded = true;
    }
    catch (const std::exception &e)
    {
        // LOG_ERROR("Error loading meals from file {}: {}", this->m_FilePath, e.what());
        this->m_IsLoaded = false;
    }
}
void StorageManager::SaveStorageToFile()
{
    try
    {
        json data = json::array();
        for (const auto &storage : this->m_Storages)
        {
            json storageJson;
            storageJson["StorageID"] = storage->GetID();

            json resourcesArray = json::array();

            for (const auto &resource : storage->GetResources())
            {
                json resourceJson;
                resourceJson["ResourceID"] = resource->GetID();
                resourceJson["ResourceName"] = resource->GetName();
                resourceJson["ResourceQuantity"] = resource->GetQuantity();
                resourceJson["ResourcePrie"] = resource->GetPrice();
                resourcesArray.push_back(resourceJson);
            }

            storageJson["Resources"] = resourcesArray;
            data.push_back(storageJson);
        }

        std::ofstream file(this->m_FilePath);
        if (!file.is_open())
        {
            // LOG_ERROR("Cannot open file for writing: {}", this->m_FilePath);
            return;
        }
        file << data.dump(4);
        file.close();
        // LOG_INFO("Successfully saved {} storages to {}", this->m_Storages.size(), this->m_FilePath);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
bool StorageManager::IsLoaded() const
{
    return this->m_IsLoaded;
}