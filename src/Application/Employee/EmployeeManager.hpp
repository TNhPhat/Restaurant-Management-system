#pragma once
#include"../../Domain/Entity/Employee/Employee.hpp"
#include"../../Infrastructure/FileHandle/FileHandle.hpp"
#include"../../Core/Utils/IDManager.hpp"
#include"../../Infrastructure/Logging/Logger.hpp"
#include"../../Domain/Service/Employee/EmployeeService.hpp"
#include<vector>

class EmployeeManager {
public:
    ~EmployeeManager() = default;

    EmployeeManager();

    bool CheckIDExist(const int &ID);

    void AddEmployee(std::shared_ptr<Employee> Employee);

    std::vector<std::shared_ptr<Employee> > GetAllEmployeeInfo();

    std::shared_ptr<Employee> SearchEmployeebyID(const int &ID);

    std::vector<std::shared_ptr<Employee> > SearchEmployeebyName(const std::string &Name);

    std::vector<std::shared_ptr<Employee> > SearchEmployeebyPosition(const EmployeePosition &Position);

    void RemoveEmployeebyID(const int &ID);

    void SaveEmployeeInfo();

    void CheckIn(const int &ID, const DateTime &Time);

    void CheckOut(const int &ID, const DateTime &Time);

    float CalcTotalWorkedHoursByID(const int &ID, const DateTime &Start, const DateTime &End);

    void SaveAttendanceRecord();

private:
    std::unique_ptr<AttendanceService> m_Attendance;
    std::unique_ptr<EmployeeInfoService> m_Info;
};
