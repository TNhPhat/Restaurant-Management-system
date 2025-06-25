#pragma once
#include"Employee.hpp"
class Receptionist: public Employee{
public:
    Receptionist();
    Receptionist(const std::string& Name,const std::string &Email,const std::string &Phone, 
        const int &EmployeeID,const DateTime &DateJoined,const int &Salary);
    ~Receptionist() = default;
    bool CreateReservation();
};