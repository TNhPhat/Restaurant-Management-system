#include"EmployeeManager.hpp"

EmployeeManager::EmployeeManager(){
    this->m_Info = std::make_unique<EmployeeInfoService>();
    this->m_Attendance = std::make_unique<AttendanceService>();
}


bool EmployeeManager::CheckIDExist(const int& ID){
    if(this->m_Info->SearchEmployeebyID(ID) != nullptr)
        return true;
    LOG_ERROR("Employee with ID {} does not exist",std::to_string(ID));
    return false;
}

void EmployeeManager::AddEmployee(std::shared_ptr<Employee> Employee){
    this->m_Info->AddEmployee(Employee);
}

std::vector<std::shared_ptr<Employee>> GetAllEmployeeInfo(){
    return this->m_Info->GetAllEmployeeInfo();
}

void EmployeeManager::SaveEmployeeInfo(){
    this->m_Info->SaveEmployeeInfo();
}   

std::shared_ptr<Employee> EmployeeManager::SearchEmployeebyID(const int& ID){
    return this->m_Info->SearchEmployeebyID(ID);
}

std::vector<std::shared_ptr<Employee>> EmployeeManager::SearchEmployeebyName(const std::string& Name){
    return this->m_Info->SearchEmployeebyName(Name);
}

std::vector<std::shared_ptr<Employee>> EmployeeManager::SearchEmployeebyPosition(const EmployeePosition& Position){
    return this->m_Info->SearchEmployeebyPosition(Position);
}

void EmployeeManager::RemoveEmployeebyID(const int& ID){
    this->m_Info->RemoveEmployeebyID(ID);
}

void EmployeeManager::CheckIn(const int& ID,const DateTime& Time){
    if(!CheckIDExist(ID))
        return;
    this->m_Attendance->CheckIn(ID,Time);
}

void EmployeeManager::CheckOut(const int& ID,const DateTime& Time){
    if(!CheckIDExist(ID))
        return;
    this->m_Attendance->CheckOut(ID,Time);
}

float EmployeeManager::CalcTotalWorkedHoursByID(const int& ID,const DateTime& Start,const DateTime& End){
    if(!CheckIDExist(ID))
        return 0;
    return this->m_Attendance->CalcTotalWorkedHoursByID(ID,Start,End);
}

void EmployeeManager::SaveAttendanceRecord(){
    this->m_Attendance->SaveAttendanceRecord();
}

