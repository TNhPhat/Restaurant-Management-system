#include"Employee.hpp"
Employee::Employee(): Person(), m_EmployeeID(0),m_DateJoined(){}
Employee::Employee(const std::string& Name,const std::string &Email,const std::string &Phone, const Gender& Gender,
    const int &EmployeeID,const DateTime &DateJoined,const int &Salary,const EmployeePosition& Position):
    Person(Name, Email, Phone, Gender), m_EmployeeID(EmployeeID), m_DateJoined(DateJoined), m_Salary(Salary), m_Position(Position) {}

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
json Employee::EmployeeToJson(const int& num) const{
    json Info;
    std::string n = NumToString(this->m_EmployeeID);
    Info[n]["ID"] = this->m_EmployeeID;
    Info[n]["Name"] = this->GetName();
    Info[n]["Gender"] = this->GenderToString();
    Info[n]["Phone"] = this->GetPhone();
    Info[n]["Email"] = this->GetEmail();
    Info[n]["Salary"] = this->m_Salary;
   // Info[n]["DateJoined"] = Date to string
    Info[n]["Position"] = this->PositionToString();
    return Info;
}

int Employee::GetYearOfService() {
     auto now = std::chrono::system_clock::now();
     std::chrono::year_month_day CurrentDate = std::chrono::floor<std::chrono::days>(now);
     return (static_cast<int>(CurrentDate.year()) - this->m_DateJoined.GetYear());
}