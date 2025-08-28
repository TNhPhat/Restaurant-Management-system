#pragma once
#include"iostream"
#include"../../ValueObject/DateTime.hpp"
#include"../../../Infrastructure/FileHandle/FileHandle.hpp"
#include"../../../Infrastructure/Logging/Logger.hpp"
#include"../../../Infrastructure/Employee/EmployeeRepository.hpp"
#include"../../Entity/Employee/Employee.hpp"
#include"../../../Core/Utils/IDManager.hpp"
#include<map>


class AttendanceService {
private:
    std::unique_ptr<EmployeeRepositoryAttendance> m_repo;
    std::vector<std::shared_ptr<AttendanceRecord> > m_record;
    std::map<int, DateTime> m_CheckInRecord;

public:
    AttendanceService();

    ~AttendanceService() = default;

    void CheckIn(const int &ID, const DateTime &Time);

    void CheckOut(const int &ID, const DateTime &Time);

    float CalcTotalWorkedHoursByID(const int &ID, const DateTime &Start, const DateTime &End);

    void SaveAttendanceRecord();
};

class EmployeeInfoService {
private:
    std::unique_ptr<EmployeeRepositoryInfo> m_repo;
    std::vector<std::shared_ptr<Employee> > m_EmployeeList;

public:
    EmployeeInfoService();

    void AddEmployee(std::shared_ptr<Employee> Employee);
    std::vector<std::shared_ptr<Employee>> GetAllEmployeeInfo();
    std::shared_ptr<Employee> SearchEmployeebyID(const int& ID);
    std::vector<std::shared_ptr<Employee>> SearchEmployeebyName(const std::string& Name);
    std::vector<std::shared_ptr<Employee>> SearchEmployeebyPosition(const EmployeePosition& Position);
    void RemoveEmployeebyID(const int& ID);
    void SaveEmployeeInfo();
};
