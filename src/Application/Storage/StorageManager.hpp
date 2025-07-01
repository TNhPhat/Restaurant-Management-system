#pragma once
#include <FileHandle.hpp>
#include <Storage.hpp>
#include <Logger.hpp>

class StorageManager
{
private:
    StorageManager();
    ~StorageManager();
    std::vector<std::shared_ptr<Storage>> m_Storages;
    std::unique_ptr<JsonHandle> m_FileHandle;
    std::string m_FilePath;
    bool m_IsLoaded;
public:
    void SetFilePath(const std::string FilePath);
    bool LoadStorageFromFile();
    void SaveStorageToFile();
    bool IsLoaded() const;
};

