#include"Manager.hpp"
Manager::Manager(): Employee() {}

Manager::Manager(const std::string& Name,const std::string &Email, const std::string &Phone, 
    const int &EmployeeID,const DateTime &DateJoined,const int &Salary):
        Employee(Name, Email, Phone, EmployeeID, DateJoined, Salary) {}
        
bool Manager::AddEmployee(){
    std::cout << "Manager " << this->GetName() << " added an employee.\n";
    return true;
}
