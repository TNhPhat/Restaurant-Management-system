#include "TestScreen.hpp"
#include "imgui.h"
#include "UI/UICore/Core.hpp"

TestScreenA::TestScreenA(Core &core): Screen(core) {
}

void TestScreenA::Render(float DT) {
    ImGui::Text("This is a test screen A.");
    if (ImGui::Button("Go to Screen B")) {
        this->m_Core.PushScreen<TestScreenB>(m_Core);
    }
}

void TestScreenA::Init() {
    m_Core.GetStyle().ScaleAllSizes(2.0f);
}

void TestScreenA::OnExit() {
}

TestScreenB::TestScreenB(Core &core): Screen(core) {
}

void TestScreenB::Render(float DT) {
    ImGui::Text("This is a test screen B.");
    if (ImGui::Button("Go to Screen A")) {
        this->m_Core.PushScreen<TestScreenA>(m_Core);
    }
}

void TestScreenB::Init() {
}

void TestScreenB::OnExit() {
}
