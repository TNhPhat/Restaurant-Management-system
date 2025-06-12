#pragma once
#include"Employee.hpp"
#include"../../Observers/UsingResourceSubject.hpp"

class Chef: public Employee ,public IObserver{
public:
    Chef();
    ~Chef();
    Chef(const std::string& Name, const std::string& Email, const std::string& Phone,
        const int& EmployeeID,const int& Salary, const Date& DateJoined, UsingResourceSubject* Subject);
    bool Update() override;
    bool TakeOrder();

private:
    UsingResourceSubject* m_Sub;
};