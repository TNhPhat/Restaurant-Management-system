#pragma once;
#include <iostream>
#include <string>
#include "../../Interface/Person.hpp"
#include "../../Shared/Date.hpp"
class Employee : public Person {
public:
    Employee();
    Employee(const std::string& Name,const std::string &Email,const std::string &Phone,const int &EmployeeID,const Date &DateJoined,const int &Salary);
    ~Employee() = default;
    int GetEmployeeID() const;
    Date GetDateJoined() const;
    void SetEmployeeID(const int &EmployeeID);
    void SetDateJoined(const Date &DateJoined);

private:
    int m_EmployeeID;
    Date m_DateJoined;
    int m_Salary;
};
