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
    IDManager::Init("/home/hlt/Documents/Restaurant-Management-system/Data/IDRegistry.json");
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
        m_EmployeeManager->SaveEmployeeInfo();
    }
}

void EmployeesScreen::DrawBackButton() {
    if (ImGui::Button("Back")) {
        this->m_Core.PopScreen();
    }
}

// void EmployeesScreen::DrawAddButton() {
//     if (ImGui::Button("Add")) {
//         // Lấy danh sách nhân viên hiện tại để tạo ID mới
//         std::vector<std::shared_ptr<Employee>> currentEmployees = m_EmployeeManager->GetAllEmployeeInfo();
//         int newId = IDManager::GetInstance();
//
//         // Tạo một đối tượng Employee mới với các giá trị mặc định/trống
//         auto newEmployee = std::make_shared<Employee>();
//
//         // Đặt ID duy nhất cho nhân viên mới
//         newEmployee->SetEmployeeID(newId);
//
//         // Thêm trực tiếp nhân viên mới vào Manager
//         m_EmployeeManager->AddEmployee(newEmployee);
//     }
// }

void EmployeesScreen::DrawAddButton() {
    if (ImGui::Button("Add")) {
        // Initialize fields with default values
        this->m_nameField = "";
        this->m_emailField = "";
        this->m_phoneField = "";
        this->m_genderField = 0; // Female = 0, Male = 1
        this->m_salaryField = 3000000; // Default salary
        this->m_positionField = 3; // Waiter = 3
        this->m_dateJoinedField = DateTime::Now();
        ImGui::OpenPopup("Add Employee");
    }

    if (ImGui::BeginPopupModal("Add Employee", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Add Employee");
        ImGui::Separator();

        ImGui::TextUnformatted("Name");
        ImGui::InputText("##Name", &m_nameField);

        ImGui::TextUnformatted("Email");
        ImGui::InputText("##Email", &m_emailField);

        ImGui::TextUnformatted("Phone");
        ImGui::InputText("##Phone", &m_phoneField);

        ImGui::TextUnformatted("Gender");
        const char *genders[] = {"Female", "Male"};
        ImGui::Combo("##Gender", &m_genderField, genders, IM_ARRAYSIZE(genders));

        ImGui::TextUnformatted("Salary");
        ImGui::InputInt("USD##Salary", &m_salaryField, 100000, 500000);
        if (m_salaryField < 0) m_salaryField = 0;

        ImGui::TextUnformatted("Position");
        const char *positions[] = {"Chef", "Manager", "Receptionist", "Waiter", "Reservation"};
        ImGui::Combo("##Position", &m_positionField, positions, IM_ARRAYSIZE(positions));

        ImGui::TextUnformatted("Date Joined");
        std::string buttonText = m_dateJoinedField.ToStringDate();
        if (ImGui::Button(buttonText.c_str())) {
            ImGui::OpenPopup("DatePicker");
        }
        if (m_datePicker.Render("DatePicker")) {
            m_dateJoinedField = m_datePicker.GetDateTime();
        }

        ImGui::Dummy(ImVec2(0.0f, 20.0f));

        if (ImGui::Button("OK", ImVec2(120, 0))) {
            // Create new employee
            auto newEmployee = std::make_shared<Employee>(
                m_nameField,
                m_emailField,
                m_phoneField,
                static_cast<Gender>(m_genderField),
                m_dateJoinedField,
                m_salaryField,
                static_cast<EmployeePosition>(m_positionField)
            );

            // Add through manager
            m_EmployeeManager->AddEmployee(newEmployee);

            // Set refresh flag if available
            this->m_shouldRefresh = true;

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }
}
