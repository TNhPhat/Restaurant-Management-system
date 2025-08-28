#pragma once
#include "imgui.h"

class Core;

class Screen {
protected:
    Core &m_Core;

public:
    Screen(Core &core): m_Core(core) {
    };

    virtual ~Screen() = default;

    virtual void Render(float DT) = 0;

    virtual void Init() = 0;

    virtual void OnExit() = 0;
};
