#pragma once

#include "FileHandle.hpp" // or wherever your JsonHandle is
#include <string>
#include <unordered_map>

class IDManager {
public:
    IDManager(const std::string& FilePath, JsonHandle* FileHandler);
    int GetNextID(const std::string& EntityType); // e.g. "Order", "Employee"
    void Save(); // persist changes

private:
    JsonHandle* m_FileHandler;
    std::unordered_map<std::string, int> m_CurrentIDs;
    std::string m_FilePath;

    void Load();
};