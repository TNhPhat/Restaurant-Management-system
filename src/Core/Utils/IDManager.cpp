#include "IDManager.hpp"
#include "Logger.hpp"

std::unique_ptr<IDManager> IDManager::s_Instance = nullptr;

IDManager::IDManager(const std::string& FilePath)
    : m_FilePath(FilePath) {
    this->m_FileHandler = std::make_unique<JsonHandle>();
    m_FileHandler->LoadFile(FilePath);
    LoadID();
}

void IDManager::Init(const std::string &FilePath) {
    if (s_Instance) {
        LOG_ERROR("IDManager already initialized!");
        return;
    }
    s_Instance = std::unique_ptr<IDManager>(new IDManager(FilePath));
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

    SaveID();
    return id;
}

void IDManager::SaveID() {
    m_FileHandler->SaveFile();
}

IDManager &IDManager::GetInstance() {
    if (!s_Instance) {
        LOG_CRITICAL("IDManager::GetInstance called before Init()");
        throw std::runtime_error("IDManager not initialized. Call IDManager::Init() first.");
    }
    return *s_Instance;
}
