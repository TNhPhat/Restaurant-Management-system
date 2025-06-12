#include"Chef.hpp"
Chef::Chef(): Employee(), m_Sub(nullptr) {}

Chef::Chef(const std::string& Name, const std::string& Email, const std::string& Phone,
        const int& EmployeeID, const int& Salary, const DateTime& DateJoined, UsingResourceSubject* Sub)
        : Employee(Name, Email, Phone, EmployeeID, DateJoined, Salary), m_Sub(Sub) 
{
    if(this->m_Sub)
        m_Sub->Attach(this);
}

Chef::~Chef(){
    if(m_Sub) m_Sub->Detach(this);
}

bool Chef::Update(){
    std::cout << "Chef " << this->GetName() << " received a resource update.\n";
    return true;
}

bool Chef::TakeOrder() {
    std::cout << "Chef " << this->GetName() << " is taking an order.\n";
    return true;
}



