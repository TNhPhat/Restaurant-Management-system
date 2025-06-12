#include "IDManager.hpp"
#include "../Logging/Logger.hpp"
#include <vector>

IDManager::IDManager(const std::string& FilePath, JsonHandle* FileHandler)
    : m_FilePath(FilePath), m_FileHandler(FileHandler) {
    m_FileHandler->LoadFile(FilePath);
    Load();
}

void IDManager::Load() {
    json data = m_FileHandler->GetDaTa();

    for (auto it = data.begin(); it != data.end(); ++it) {
        m_CurrentIDs[it.key()] = it.value().get<int>();
    }
}

int IDManager::GetNextID(const std::string& entityType) {
    int& id = m_CurrentIDs[entityType];
    id += 1;

    std::vector<std::string> key = { entityType };
    m_FileHandler->ExecuteCommand(new SetKeyCommand(key, id));

    m_FileHandler->SaveFile();
    return id;
}

void IDManager::Save() {
    m_FileHandler->SaveFile();
}
