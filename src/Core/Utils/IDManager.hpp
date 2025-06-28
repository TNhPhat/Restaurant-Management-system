#pragma once

#include "FileHandle.hpp" 
#include <string>
#include <unordered_map>

class IDManager {
private:
    static std::unique_ptr<IDManager> s_Instance;

    JsonHandle* m_FileHandler;
    std::unordered_map<std::string, int> m_CurrentIDs;
    std::string m_FilePath;
    
    void LoadID();

    IDManager(const std::string& FilePath, JsonHandle* FileHandler);

public:
    static void Init(const std::string& FilePath, JsonHandle* FileHandler);
    static IDManager &GetInstance();

    int GetNextID(const std::string& EntityType); 
    void SaveID(); 
};