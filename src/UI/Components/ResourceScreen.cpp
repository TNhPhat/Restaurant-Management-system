#include "ResourceScreen.hpp"

#include <iostream>

#include "UI/UICore/Core.hpp"

ResourceScreen::ResourceScreen(Core &core, Storage *storage)
: Screen(core), m_Storage(storage),
m_Resources(storage->GetResources()) {}


void ResourceScreen::Init(){

}
void ResourceScreen::OnExit(){

}
void ResourceScreen::Render(float dt){
    ImGui::Text("Storage Resources Management");
    DrawSaveButton();
    ImGui::SameLine();
    DrawAddButton();
    ImGui::SameLine();
    DrawBackButton();

    DrawTable();
}

void ResourceScreen::DrawTable() {
    if (ImGui::BeginTable("Resources", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Index");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Quantity");
        ImGui::TableSetupColumn("Price");
        ImGui::TableHeadersRow();

        int num = 0;
        for (auto &resource : m_Resources) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            ImGui::Text(std::to_string(num).c_str());
            ImGui::TableSetColumnIndex(1);
            ImGui::Text((resource->GetName()).c_str());
            ImGui::TableSetColumnIndex(2);
            ImGui::Text((std::to_string(resource->GetQuantity())).c_str());
            ImGui::TableSetColumnIndex(3);
            ImGui::Text((std::to_string(resource->GetPrice())).c_str());

            ++num;
        }
        ImGui::EndTable();
    }
}
void ResourceScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        // StorageManager::GetInstance().SaveStorageToFile();

    }
}
void ResourceScreen::DrawBackButton() {
    if (ImGui::Button("Back")) {
        this->m_Core.PopScreen();
    }
}

void ResourceScreen::DrawAddButton() {
    if (ImGui::Button("Add")) {
        // this->m_Storage.push_back(std::make_shared<Storage>(this->m_Storage.back()->GetID() + 1));
    }
}
