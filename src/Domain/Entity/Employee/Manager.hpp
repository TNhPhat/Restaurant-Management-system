#pragma once
#include"Employee.hpp"
class Manager: public Employee{
public:
    Manager();
    Manager(const std::string& Name,const std::string &Email,const std::string &Phone, 
        const int &EmployeeID,const DateTime &DateJoined,const int &Salary);
    ~Manager() = default;
    bool AddEmployee();

};