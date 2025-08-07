#pragma once

#include "Screen.hpp"
#include "../../Application/Storage/StorageManager.hpp"
#include "../../Domain/Entity/Storage/Storage.hpp"


#include "imgui.h"

class StorageScreen : public Screen {
private:
public:
    StorageScreen(Core& core);
    void Init() override;
    void OnExit() override;
    void Render(float dt) override;

private:

};