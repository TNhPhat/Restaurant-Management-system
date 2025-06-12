#pragma once
#include <iostream>
#include <string>
#include "../../Interface/Person.hpp"
#include "../../ValueObject/DateTime.hpp"
class Employee : public Person {
public:
    Employee();
    Employee(const std::string& Name,const std::string &Email,const std::string &Phone,const int &EmployeeID,const DateTime &DateJoined,const int &Salary);
    ~Employee() = default;
    int GetEmployeeID() const;
    DateTime GetDateJoined() const;
    void SetEmployeeID(const int &EmployeeID);
    void SetDateJoined(const DateTime &DateJoined);

private:
    int m_EmployeeID;
    DateTime m_DateJoined;
    int m_Salary;
};
