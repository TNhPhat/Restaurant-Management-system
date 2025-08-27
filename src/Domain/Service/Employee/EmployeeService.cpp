#include"EmployeeService.hpp"

AttendanceService::AttendanceService(){
    this->m_repo = std::make_unique<EmployeeRepositoryAttendance>();
    this->m_record = this->m_repo->GetAllEmployeeAttendanceRecord();
    this->m_CheckInRecord.clear();
}

void AttendanceService::CheckIn(const int& ID,const DateTime& Time){
    if(this->m_CheckInRecord.find(ID) != this->m_CheckInRecord.end()){
        LOG_ERROR("Employee with ID {} has been Check In before and not Check Out yet",std::to_string(ID));
        return;
    }
    this->m_CheckInRecord[ID] = Time;
    LOG_INFO("Employee with ID {} Check In at {} ",std::to_string(ID),Time.ToStringDateTime());
}

void AttendanceService::CheckOut(const int& ID, const DateTime& Time){
    if(this->m_CheckInRecord.find(ID) == this->m_CheckInRecord.end()){
        LOG_ERROR("Employee with ID {} has not been Check In",std::to_string(ID));
        return;
    }
    std::shared_ptr<AttendanceRecord> record = std::make_shared<AttendanceRecord>(ID);
    record->CheckIn(this->m_CheckInRecord[ID]);
    record->CheckOut(Time);
    m_record.push_back(record);
    this->m_CheckInRecord.erase(this->m_CheckInRecord.find(ID));
    LOG_INFO("Employee with ID {} Check Out at {} ",std::to_string(ID),Time.ToStringDateTime());
}

float AttendanceService::CalcTotalWorkedHoursByID(const int& ID,const DateTime& Start,const DateTime& End){
    float TotalWorkedHours = 0.0f;
    for(auto it : this->m_record)
        if(it->GetID() == ID){
            if(it->GetCheckInTime() >= Start && it->GetCheckInTime() <= End)
                TotalWorkedHours += it->GetWorkedHours();
        }
    return TotalWorkedHours;
}

void AttendanceService::SaveAttendanceRecord(){
    m_repo->SaveEmployeeAttendaceRecord(m_record);
}


EmployeeInfoService::EmployeeInfoService(){
    this->m_repo = std::make_unique<EmployeeRepositoryInfo>();
    this->m_EmployeeList.clear();
    this->m_EmployeeList = this->m_repo->GetAllEmployeeInfo();
}

void EmployeeInfoService::AddEmployee(std::shared_ptr<Employee> Employee){
    Employee->SetEmployeeID(IDManager::GetInstance().GetNextID("Employee"));
    this->m_EmployeeList.push_back(Employee);
}

std::vector<std::shared_ptr<Employee>> EmployeeInfoService::GetAllEmployeeInfo(){
    return this->m_EmployeeList;    
}

void EmployeeInfoService::SaveEmployeeInfo(){
    this->m_repo->SaveEmployeeInfo(this->m_EmployeeList);
}   

std::shared_ptr<Employee> EmployeeInfoService::SearchEmployeebyID(const int& ID){
    for(auto it: this->m_EmployeeList)
        if(it->GetEmployeeID() == ID)
            return it;
    return nullptr;
}

std::vector<std::shared_ptr<Employee>> EmployeeInfoService::SearchEmployeebyName(const std::string& Name){
    std::vector<std::shared_ptr<Employee>> res;
    for(auto it: this->m_EmployeeList)
        if(it->GetName() == Name)
            res.push_back(it);
    return res;
}

std::vector<std::shared_ptr<Employee>> EmployeeInfoService::SearchEmployeebyPosition(const EmployeePosition& Position){
    std::vector<std::shared_ptr<Employee>> res;
    for(auto it: this->m_EmployeeList)
        if(it->GetPostion() == Position)
            res.push_back(it);
    return res;
}

void EmployeeInfoService::RemoveEmployeebyID(const int& ID){
    for(auto it = this->m_EmployeeList.begin() ; it != this->m_EmployeeList.end() ; it++)
        if((*it)->GetEmployeeID() == ID){
            this->m_EmployeeList.erase(it);
            LOG_INFO("Successfull remove Employee with ID {} from employee list",std::to_string(ID));
            return;
        }
        LOG_INFO("No Employee with ID {} to remove",std::to_string(ID));
}
