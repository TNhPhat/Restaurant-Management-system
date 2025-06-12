#include"Employee.hpp"
Employee::Employee(): Person(), m_EmployeeID(0),m_DateJoined(){}
Employee::Employee(const std::string& Name,const std::string &Email,const std::string &Phone, 
    const int &EmployeeID,const DateTime &DateJoined,const int &Salary):
    Person(Name, Email, Phone), m_EmployeeID(EmployeeID), m_DateJoined(DateJoined), m_Salary(Salary) {}

void Employee::SetEmployeeID(const int &EmployeeID){
    this->m_EmployeeID = EmployeeID;
}

void Employee::SetDateJoined(const DateTime &DateJoined){
    this->m_DateJoined = DateJoined;
}

int Employee::GetEmployeeID() const{
    return this->m_EmployeeID;
}

DateTime Employee::GetDateJoined() const{
    return this->m_DateJoined;
}
