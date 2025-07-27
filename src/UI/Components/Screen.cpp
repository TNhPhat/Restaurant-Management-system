#include "Screen.hpp"

Screen::Screen(ImGuiIO &io): m_IO(io) {
}

DemoScreen::DemoScreen(ImGuiIO &io): Screen(io) {
}

void DemoScreen::Render(float DT) {
    if (show_demo_window) {
        ImGui::ShowDemoWindow(&show_demo_window);
    }
    ImGui::Begin("Demo Screen");
    ImGui::Text("This is a demo screen.");
    ImGui::Checkbox("Show Demo Window", &show_demo_window);
    ImGui::Checkbox("Other Window", &show_another_window);
    ImGui::SliderFloat("Demo Slider", &f, 0.0f, 1.0f);
    if (ImGui::Button("Button"))
        ++counter;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / m_IO.Framerate, m_IO.Framerate);
    ImGui::End();
    if (show_another_window) {
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me")) {
            show_another_window = false;
        }
        ImGui::End();
    }
}

void DemoScreen::OnExit() {
}

void DemoScreen::Init() {
}
