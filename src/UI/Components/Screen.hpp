#pragma once
#include "imgui.h"

class Core;

class Screen {
protected:
    Core &m_Core;

public:
    Screen(Core &core);

    virtual ~Screen() = default;

    virtual void Render(float DT) = 0;

    virtual void Init() = 0;

    virtual void OnExit() = 0;
};

class DemoScreen : public Screen {
private:
    int counter = 0;
    bool show_demo_window = true;
    bool show_another_window = false;
    float f = 0.0f;

public:
    DemoScreen(Core &core);

    void Render(float DT) override;

    void OnExit() override;

    void Init() override;
};
