#pragma once

#include "FileHandle.hpp" // or wherever your JsonHandle is
#include <string>
#include <unordered_map>

class IDManager {
private:
    static IDManager *s_Instance;
    JsonHandle* m_FileHandler;
    std::unordered_map<std::string, int> m_CurrentIDs;
    std::string m_FilePath;
    
    void LoadID();

public:
    IDManager(const std::string& FilePath, JsonHandle* FileHandler);
    int GetNextID(const std::string& EntityType); 
    void SaveID(); 
    static void SetInstance(IDManager *Instance);
    static IDManager &GetInstance();

};