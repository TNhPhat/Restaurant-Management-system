#pragma once
#include "Screen.hpp"
#include "../../Domain/Service/Employee/EmployeeService.hpp"

#include <vector>
#include <memory>

class EmployeesScreen : public Screen {
public:
    EmployeesScreen(Core& core); // <-- Bỏ Company* khỏi constructor
    ~EmployeesScreen() = default;

    void Init() override;
    void OnExit() override;
    void Render(float dt) override;

private:
    void DrawTable();
    void DrawSaveButton();
    void DrawAddButton();
    void DrawBackButton();

private:

    std::vector<std::shared_ptr<Employee>> m_Employees;
};