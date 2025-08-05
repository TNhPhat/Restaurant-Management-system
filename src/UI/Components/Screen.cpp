#include "Screen.hpp"
#include "imgui.h"
#include "UI/UICore/Core.hpp"

Screen::Screen(Core &core): m_Core(core) {
}

DemoScreen::DemoScreen(Core &core): Screen(core) {
}

void DemoScreen::Render(float DT) {
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    ImGui::Text("This is a demo asfajgasjfgsklagaskjldgbablgaldfgabjlgjlker.");
    ImGui::Checkbox("Show znsdnskdgjdgbdj Window", &show_demo_window);
    ImGui::Checkbox("Other Window", &show_another_window);
    ImGui::SliderFloat("Demo Slider", &f, 0.0f, 1.0f);
    if (ImGui::Button("Button"))
        ++counter;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_Core.GetIO().Framerate,
                m_Core.GetIO().Framerate);
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("NONO Me")) {
            show_another_window = false;
        }
        ImGui::End();
    }
}

void DemoScreen::OnExit() {
}

void DemoScreen::Init() {
}

// AAA