#include "StorageScreen.hpp"

#include "ResourceScreen.hpp"
#include "UI/UICore/Core.hpp"

StorageScreen::StorageScreen(Core &core)
: Screen(core), m_Storages(StorageManager::GetInstance().GetStorages())
{
    StorageManager::GetInstance().SetFilePath("/home/hlt/Documents/Restaurant-Management-system/Data/Storages.json");
    StorageManager::GetInstance().LoadStorageFromFile();

    m_Storages = StorageManager::GetInstance().GetStorages();
}

void StorageScreen::Init(){

}
void StorageScreen::OnExit(){
    StorageManager::GetInstance().SaveStorageToFile();
}
void StorageScreen::Render(float dt){
    ImGui::Text("Restaurant Storage Management");
    DrawSaveButton();
    ImGui::SameLine();
    DrawAddButton();
    ImGui::SameLine();
    DrawBackButton();

    DrawTable();
}

void StorageScreen::DrawTable() {
    if (ImGui::BeginTable("Storage", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthStretch, 4.0);
        ImGui::TableSetupColumn("Options");
        ImGui::TableHeadersRow();

        int num = 0;
        for (auto &storage : m_Storages) {
            ImGui::TableNextRow();
            ImGui::TableSetColumnIndex(0);
            std::string text = "Storage " + std::to_string(num);
            if (ImGui::Selectable(text.c_str(), true, ImGuiSelectableFlags_None))
            {
                std::cout << "Click on " << num << std::endl;
                StorageManager::GetInstance().SaveStorageToFile();
                this->m_Core.PushScreen(std::make_unique<ResourceScreen>(m_Core, storage.get()));
            }
            ImGui::TableSetColumnIndex(1);
            if (ImGui::Button(("Delete##" + std::to_string(num)).c_str())) {
                auto deleted = m_Storages[num];
                m_Storages.erase(m_Storages.begin() + num);
                std::cout << m_Storages.size() << std::endl;
            }
            ++num;
        }
        ImGui::EndTable();
    }
}
void StorageScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        StorageManager::GetInstance().SaveStorageToFile();

    }
}
void StorageScreen::DrawBackButton() {
    if (ImGui::Button("Back")) {
        this->m_Core.PopScreen();
    }
}

void StorageScreen::DrawAddButton() {
    if (ImGui::Button("Add")) {
        this->m_Storages.push_back(std::make_shared<Storage>(this->m_Storages.back()->GetID() + 1));
    }
}
