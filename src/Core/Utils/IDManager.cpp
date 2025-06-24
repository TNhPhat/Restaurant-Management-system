#include "IDManager.hpp"
#include "../Logging/Logger.hpp"
#include <vector>

IDManager *IDManager::s_Instance = nullptr;

IDManager::IDManager(const std::string& FilePath, JsonHandle* FileHandler)
    : m_FilePath(FilePath), m_FileHandler(FileHandler) {
    m_FileHandler->LoadFile(FilePath);
    LoadID();
}

void IDManager::LoadID() {
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

void IDManager::SaveID() {
    m_FileHandler->SaveFile();
}

void IDManager::SetInstance(IDManager *Instance) {
    s_Instance = Instance;
}

IDManager &IDManager::GetInstance() {
    if (!s_Instance) {
        Logger::Log(LogLevel::CRITICAL, __FILE__, __LINE__, 
            "IDManager::getInstance called before initialization. Call setInstance() first.");
        // You can still choose to throw afterwards:
        throw std::runtime_error("IDManager not initialized.");
    }
    return *s_Instance;
}
