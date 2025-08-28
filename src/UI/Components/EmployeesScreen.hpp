#pragma once
#include "Screen.hpp"
#include "../../Domain/Service/Employee/EmployeeService.hpp"

#include <vector>
#include <memory>

#include "Application/Employee/EmployeeManager.hpp"
#include "DateTimePicker.hpp"
#include "misc/cpp/imgui_stdlib.h"
//
// class EmployeesScreen : public Screen {
// public:
//     EmployeesScreen(Core &core);
//     ~EmployeesScreen() = default;
//
//     void Init() override;
//
//     void OnExit() override;
//
//     void Render(float dt) override;
//
// private:
//     void DrawTable();
//
//     void DrawSaveButton();
//
//     void DrawAddButton();
//
//     void DrawBackButton();
//
//     std::shared_ptr<EmployeeManager> m_EmployeeManager = nullptr;
// };

class EmployeesScreen : public Screen {
public:
    EmployeesScreen(Core &core); // <-- Bỏ Company* khỏi constructor
    ~EmployeesScreen() = default;

    void Init() override;

    void OnExit() override;

    void Render(float dt) override;

private:
    void DrawTable();

    void DrawSaveButton();

    void DrawAddButton();

    void DrawBackButton();

    std::shared_ptr<EmployeeManager> m_EmployeeManager = nullptr;

    // Add Employee popup fields
    std::string m_nameField;
    std::string m_emailField;
    std::string m_phoneField;
    int m_genderField = 0;
    int m_salaryField = 3000000;
    int m_positionField = 3;
    DateTime m_dateJoinedField = DateTime::Now();
    DateTimePicker m_datePicker;
    bool m_shouldRefresh = true;
};