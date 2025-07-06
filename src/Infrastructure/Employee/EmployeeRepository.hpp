#pragma once
#include<iostream>
#include"../FileHandle/FileHandle.hpp"
#include"../../Domain/Entity/Employee/Employee.hpp"

class AttendanceRecord{
private:
    int m_ID;
    std::shared_ptr<DateTime> CheckInTime;
    std::shared_ptr<DateTime> CheckOutTime;
    float m_TotalWorkedHours;
public:
    AttendanceRecord(const int& ID);
    void CheckIn(const DateTime& Time);
    void CheckOut(const DateTime& Time);
    int GetID() const;
    DateTime GetCheckInTime() const;
    DateTime GetCheckOutTime() const;
    float GetWorkedHours() const;  
    json ToJson() const;
};

class EmployeeRepositoryInfo{
public:
    ~EmployeeRepositoryInfo() = default;
    std::vector<std::shared_ptr<Employee>> GetAllEmployeeInfo();
    void SaveEmployeeInfo(std::vector<std::shared_ptr<Employee>>& EmployeeInfo);
    EmployeeRepositoryInfo();
    JsonHandle& GetFileHandler();

private:
    std::unique_ptr<JsonHandle> m_FileHandler;
};
 
class EmployeeRepositoryAttendance{
public:
    ~EmployeeRepositoryAttendance() = default;
    std::vector<std::shared_ptr<AttendanceRecord>> GetAllEmployeeAttendanceRecord();
    void SaveEmployeeAttendaceRecord(std::vector<std::shared_ptr<AttendanceRecord>>& record);
    JsonHandle& GetFileHandler();
    EmployeeRepositoryAttendance();
private:
    std::unique_ptr<JsonHandle> m_FileHandler;
};