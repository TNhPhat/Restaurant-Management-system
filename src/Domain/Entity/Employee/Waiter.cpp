#include "Waiter.hpp"
Waiter::Waiter(): Employee() {}

Waiter::Waiter(const std::string& Name,const std::string &Email, const std::string &Phone, 
    const int &EmployeeID,const Date &DateJoined,const int &Salary):
        Employee(Name, Email, Phone, EmployeeID, DateJoined, Salary) {}

bool Waiter::CreateOrder(){
    std::cout << "Waiter " << this->GetName() << " created an order.\n";
    return true;
}