#include "Dashboard.hpp"

#include "IDManager.hpp"
#include "Application/Storage/StorageManager.hpp"

Dashboard::Dashboard(Core &core): Screen(core) {
}

void Dashboard::Render(float DT) {
}

void Dashboard::Init() {
    IDManager::Init("Data/IDRegistry.json");
    StorageManager::GetInstance().SetFilePath("Data/Storages.json");
    StorageManager::GetInstance().LoadStorageFromFile();
}

void Dashboard::OnExit() {
}
