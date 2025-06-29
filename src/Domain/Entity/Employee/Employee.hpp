#pragma once
#include <iostream>
#include <string>
#include "Person.hpp"
#include "DateTime.hpp"
#include "FileHandle.hpp"
#include <chrono>

enum EmployeePosition{
    Chef,
    Manager,
    Receptionist,
    Waiter,
    Reservation
};

class Employee : public Person {
public:
    Employee();
    Employee(const std::string& Name,const std::string &Email,const std::string &Phone, const Gender& Gender,
        const int &EmployeeID,const DateTime &DateJoined,const int &Salary,const EmployeePosition& Position);
    ~Employee() = default;
    int GetEmployeeID() const;
    DateTime GetDateJoined() const;
    int GetSalary() const;
    EmployeePosition GetPostion() const;
    void SetPosition(const EmployeePosition& Position);
    void SetEmployeeID(const int &EmployeeID);
    void SetDateJoined(const DateTime &DateJoined);
    void SetSalary(const int& Salary);
    std::string PositionToString() const;
    json EmployeeToJson(const int& num) const;
    int GetYearOfService() ;
    static std::string NumToString(int num);

private:
    EmployeePosition m_Position;
    int m_EmployeeID;
    DateTime m_DateJoined;
    int m_Salary;
};
