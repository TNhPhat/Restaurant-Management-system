#include "EmployeesScreen.hpp"

#include "Application/Employee/EmployeeManager.hpp"
#include "UI/UICore/Core.hpp"

// Constructor giờ đây lấy dữ liệu từ EmployeeManager::GetInstance()
EmployeesScreen::EmployeesScreen(Core &core)
    : Screen(core) {
}

void EmployeesScreen::Init() {
    // Không thay đổi
    m_EmployeeManager = std::make_shared<EmployeeManager>();
}

void EmployeesScreen::OnExit() {
    // Không thay đổi
}

void EmployeesScreen::Render(float dt) {
    ImGui::Text("Employees Management");
    DrawSaveButton();
    ImGui::SameLine();
    DrawAddButton();
    ImGui::SameLine();
    DrawBackButton();

    DrawTable();
}

void EmployeesScreen::DrawTable() {
    auto m_Employees = m_EmployeeManager->GetAllEmployeeInfo();
    if (ImGui::BeginTable("Employees", 6,
                          ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable)) {
        // ... Nội dung của DrawTable giữ nguyên như trước ...
        ImGui::TableSetupColumn("ID");
        ImGui::TableSetupColumn("Name");
        ImGui::TableSetupColumn("Position");
        ImGui::TableSetupColumn("Salary");
        ImGui::TableSetupColumn("Date Joined");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (int i = 0; i < m_Employees.size(); ++i) {
            auto &employee = m_Employees[i];
            ImGui::PushID(i);

            ImGui::TableNextRow();

            // Cột 0: ID
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%d", employee->GetEmployeeID());

            // Cột 1: Name
            ImGui::TableSetColumnIndex(1);
            char nameBuffer[128];
            strncpy(nameBuffer, employee->GetName().c_str(), sizeof(nameBuffer));
            nameBuffer[sizeof(nameBuffer) - 1] = 0;
            if (ImGui::InputText("##Name", nameBuffer, sizeof(nameBuffer))) {
                employee->SetName(nameBuffer);
            }

            // Cột 2: Position
            ImGui::TableSetColumnIndex(2);
            const char *positions[] = {"Chef", "Manager", "Receptionist", "Waiter", "Reservation"};
            int currentPosition = static_cast<int>(employee->GetPostion());
            if (ImGui::Combo("##Position", &currentPosition, positions, IM_ARRAYSIZE(positions))) {
                employee->SetPosition(static_cast<EmployeePosition>(currentPosition));
            }

            // Cột 3: Salary
            ImGui::TableSetColumnIndex(3);
            int salary = employee->GetSalary();
            if (ImGui::InputInt("##Salary", &salary, 1000, 10000)) {
                if (salary < 0) salary = 0;
                employee->SetSalary(salary);
            }

            // Cột 4: Date Joined
            ImGui::TableSetColumnIndex(4);
            ImGui::Text("%s", employee->GetDateJoined().ToStringDate().c_str());

            // Cột 5: Actions
            ImGui::TableSetColumnIndex(5);
            if (ImGui::Button("Delete")) {
                m_Employees.erase(m_Employees.begin() + i);
                --i;
            }

            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void EmployeesScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        // Gọi hàm lưu từ EmployeeManager
    }
}

void EmployeesScreen::DrawBackButton() {
    if (ImGui::Button("Back")) {
        this->m_Core.PopScreen();
    }
}

void EmployeesScreen::DrawAddButton() {
    if (ImGui::Button("Add")) {
        // Logic thêm mới không đổi, vì m_Employees là tham chiếu trực tiếp đến dữ liệu trong Manager
        // int newId = m_Employees.empty() ? 1 : m_Employees.back()->GetEmployeeID() + 1;
        // auto newEmployee = std::make_shared<Employee>("New Employee", "email@example.com", "0123456789", Gender::Female,
        //                                               DateTime::Now(), 3000000, EmployeePosition::Waiter);
        // newEmployee->SetEmployeeID(newId);
        // this->m_Employees.push_back(newEmployee);
    }
}
