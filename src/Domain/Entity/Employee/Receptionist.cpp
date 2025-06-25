#include "Receptionist.hpp"
Receptionist::Receptionist(): Employee() {}
Receptionist::Receptionist(const std::string& Name,const std::string &Email, const std::string &Phone, 
    const int &EmployeeID,const DateTime &DateJoined,const int &Salary):
        Employee(Name, Email, Phone, EmployeeID, DateJoined, Salary) {}
        
bool Receptionist::CreateReservation(){
    std::cout << "Receptionist " << this->GetName() << " created a reservation.\n";
    return true;
}
