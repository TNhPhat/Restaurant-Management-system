#pragma once

#include "Screen.hpp"
#include "../../Application/Storage/StorageManager.hpp"
#include "../../Domain/Entity/Storage/Storage.hpp"


#include "imgui.h"

class StorageScreen : public Screen {
private:
    std::vector<std::shared_ptr<Storage>> &m_Storages;
public:
    StorageScreen(Core& core);
    void Init() override;
    void OnExit() override;
    void Render(float dt) override;
private:
    void DrawSaveButton();
    void DrawBackButton();
    void DrawTable();
    void DrawAddButton();
};