#include"Employee.hpp"
Employee::Employee(): Person(), m_EmployeeID(0),m_DateJoined(){}
Employee::Employee(const std::string& Name,const std::string &Email,const std::string &Phone, const Gender& Gender,
    const DateTime &DateJoined,const int &Salary,const EmployeePosition& Position):
    Person(Name, Email, Phone, Gender), m_EmployeeID(0), m_DateJoined(DateJoined), m_Salary(Salary), m_Position(Position) {}

void Employee::SetEmployeeID(const int &EmployeeID){
    this->m_EmployeeID = EmployeeID;
}

void Employee::SetDateJoined(const DateTime &DateJoined){
    this->m_DateJoined = DateJoined;
}

void Employee::SetSalary(const int& Salary){
    this->m_Salary = Salary;
}

void Employee::SetPosition(const EmployeePosition& Position){
    this->m_Position = Position;
}

int Employee::GetEmployeeID() const{
    return this->m_EmployeeID;
}

DateTime Employee::GetDateJoined() const{
    return this->m_DateJoined;
}

int Employee::GetSalary() const{
    return this->m_Salary;
}

EmployeePosition Employee::GetPostion() const{
    return this->m_Position;
}

std::string Employee::PositionToString() const{
    switch (this->m_Position)
    {
        case EmployeePosition::Chef:
            return "Chef";
        case EmployeePosition::Manager:
            return "Manager";
        case EmployeePosition::Receptionist:
            return "Receptionist";
        case EmployeePosition::Reservation:
            return "Reservation";
        case EmployeePosition::Waiter:
            return "Waiter";
        default:
            return "";
    }
}
std::string Employee::NumToString(int num){
    std::string res="";
    while(num > 0){
        res.push_back(num%10 + '0');
        num /= 10;
    }
    std::reverse(res.begin(),res.end());
    return res;
}
json Employee::ToJson() const{
    json Info;
    Info = Person::ToJson();
    Info["ID"] = this->m_EmployeeID;
    Info["Salary"] = this->m_Salary;
    Info["DateJoined"] = this->m_DateJoined.ToStringDate();
    Info["Position"] = this->PositionToString();
    return Info;
}

int Employee::GetYearOfService() {
     auto now = std::chrono::system_clock::now();
     std::chrono::year_month_day CurrentDate = std::chrono::floor<std::chrono::days>(now);
     return (static_cast<int>(CurrentDate.year()) - this->m_DateJoined.GetYear());
}

EmployeePosition Employee::StringToPosition(const std::string& position){
    if(position == "Chef")
        return EmployeePosition::Chef;
    if(position == "Manager")
        return EmployeePosition::Manager;
    if(position == "Receptionist")
        return EmployeePosition::Receptionist;
    if(position == "Reservation")
        return EmployeePosition::Reservation;
    if(position == "Waiter")
        return EmployeePosition::Waiter;
}