#pragma once
#include <iostream>
#include <string>
#include "../../Interface/Person.hpp"
#include "../../ValueObject/DateTime.hpp"
#include "../../../Infrastructure/FileHandle/FileHandle.hpp"
#include <chrono>
#include<fstream>
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
        const DateTime &DateJoined,const int &Salary,const EmployeePosition& Position);
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
    json EmployeeToJson() const;
    int GetYearOfService() ;
    static std::string NumToString(int num);
    static EmployeePosition StringToPosition(const std::string& position);

private:
    EmployeePosition m_Position;
    int m_EmployeeID;
    DateTime m_DateJoined;
    int m_Salary;
};
