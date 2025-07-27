#pragma once
#include "imgui.h"

class Screen {
protected:
    ImGuiIO &m_IO;

public:
    Screen(ImGuiIO &io);

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
    DemoScreen(ImGuiIO &io);

    void Render(float DT) override;

    void OnExit() override;

    void Init() override;
};
