#pragma once
#include "Screen.hpp"
#include "../../Domain/Service/Employee/EmployeeService.hpp"

#include <vector>
#include <memory>

#include "Application/Employee/EmployeeManager.hpp"

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
};
