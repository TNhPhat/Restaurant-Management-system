#include "ResourceScreen.hpp"

#include <iostream>

#include "IDManager.hpp"
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
        // for (auto &resource : m_Resources) {
        //     ImGui::TableNextRow();
        //     ImGui::TableSetColumnIndex(0);
        //     ImGui::Text(std::to_string(num).c_str());
        //     ImGui::TableSetColumnIndex(1);
        //     ImGui::Text((resource->GetName()).c_str());
        //     ImGui::TableSetColumnIndex(2);
        //     ImGui::Text((std::to_string(resource->GetQuantity())).c_str());
        //     ImGui::TableSetColumnIndex(3);
        //     ImGui::Text((std::to_string(resource->GetPrice())).c_str());
        //
        //     ++num;
        // }

        for (auto& resource : m_Resources) {
            // Đẩy ID của vòng lặp vào để mỗi widget trong mỗi hàng có ID duy nhất
            ImGui::PushID(num);

            ImGui::TableNextRow();

            // Cột 0: Index (Không thay đổi)
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", num);

            // Cột 1: Name (Sử dụng InputText)
            ImGui::TableSetColumnIndex(1);
            {
                // ImGui::InputText yêu cầu một buffer char*, không phải std::string
                // Vì vậy, chúng ta tạo một buffer tạm, sao chép dữ liệu vào
                char nameBuffer[128];
                strncpy(nameBuffer, resource->GetName().c_str(), sizeof(nameBuffer));
                nameBuffer[sizeof(nameBuffer) - 1] = 0; // Đảm bảo kết thúc bằng null

                // Nếu người dùng thay đổi giá trị trong InputText
                if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
                    resource->SetName(nameBuffer); // Gọi hàm setter để cập nhật
                }
            }

            // Cột 2: Quantity (Sử dụng InputInt)
            ImGui::TableSetColumnIndex(2);
            {
                int quantity = resource->GetQuantity();
                if (ImGui::InputInt("##Quantity", &quantity)) {
                    if (quantity < 0) quantity = 0; // Tùy chọn: không cho phép số âm
                    resource->SetQuantity(quantity);
                }
            }

            // Cột 3: Price (Sử dụng InputFloat)
            ImGui::TableSetColumnIndex(3);
            {
                float price = resource->GetPrice();
                // "%.2f" để định dạng hiển thị 2 chữ số thập phân
                if (ImGui::InputFloat("##Price", &price, 0.0f, 0.0f, "%.2f")) {
                    if (price < 0.0f) price = 0.0f; // Tùy chọn: không cho phép giá âm
                    resource->SetPrice(price);
                }
            }

            // Xóa ID của hàng hiện tại khỏi stack
            ImGui::PopID();
            ++num;
        }
        ImGui::EndTable();
    }
}
void ResourceScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        StorageManager::GetInstance().SaveStorageToFile();
    }
}
void ResourceScreen::DrawBackButton() {
    if (ImGui::Button("Back")) {
        this->m_Core.PopScreen();
    }
}

void ResourceScreen::DrawAddButton() {
    if (ImGui::Button("Add")) {
        int nID = this->m_Resources.empty() ? 1 : this->m_Resources.back()->GetID() + 1;
        this->m_Resources.push_back(std::make_shared<Resource>(nID));
    }
}
