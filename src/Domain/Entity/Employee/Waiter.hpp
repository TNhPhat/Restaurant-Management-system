#pragma once
#include "Employee.hpp"
class Waiter : public Employee {
public:
    Waiter();
    Waiter(const std::string& Name,const std::string &Email,const std::string &Phone, 
        const int &EmployeeID,const DateTime &DateJoined,const int &Salary);
    ~Waiter() = default;
    bool CreateOrder();
};
