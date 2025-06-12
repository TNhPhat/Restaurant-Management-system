#pragma once 

#include <unordered_map>
#include <string>
#include "../Infrastructure/FileHandle/FileHandle.h"

class IDManager {
private:
    nlohmann::json IDData;
    std::string Filename;

    void LoadData();
    void SaveData();

public:
    IDManager(const std::string& File = "id_registry.json");
    int GetNextID(const std::string& Key);
};