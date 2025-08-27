#pragma once
#include "Screen.hpp"
#include "Storage/Storage.hpp"
#include "Storage/Resource.hpp"

class ResourceScreen : public Screen {
private:
    Storage * m_Storage;
    std::vector<std::shared_ptr<Resource>> &m_Resources;
public:
    ResourceScreen(Core &core, Storage * storage);
    void Init() override;
    void OnExit() override;
    void Render(float dt) override;
private:
    void DrawSaveButton();
    void DrawBackButton();
    void DrawTable();
    void DrawAddButton();
};