#include"EmployeeRepository.hpp"

AttendanceRecord::AttendanceRecord(const int &ID): m_ID(ID) {
    CheckInTime = nullptr;
    CheckOutTime = nullptr;
}

void AttendanceRecord::CheckIn(const DateTime &Time) {
    this->CheckInTime = std::make_shared<DateTime>(Time);
}

void AttendanceRecord::CheckOut(const DateTime &Time) {
    if (!this->CheckInTime) {
        LOG_ERROR("CheckInTime not set");
        return;
    }
    this->CheckOutTime = std::make_shared<DateTime>(Time);
    time_t start = CheckInTime->ToTimeT();
    time_t end = CheckOutTime->ToTimeT();
    if (end <= start) {
        LOG_ERROR("CheckOutTime smaller than CheckInTime");
        return;
    }
    double durationSec = std::difftime(end, start);
    this->m_TotalWorkedHours = static_cast<float>(durationSec / 3600.0);
}

int AttendanceRecord::GetID() const {
    return this->m_ID;
}

DateTime AttendanceRecord::GetCheckInTime() const {
    return *this->CheckInTime;
}

DateTime AttendanceRecord::GetCheckOutTime() const {
    return *this->CheckOutTime;
}

float AttendanceRecord::GetWorkedHours() const {
    return this->m_TotalWorkedHours;
}

json AttendanceRecord::ToJson() const {
    json res;
    res["ID"] = this->m_ID;
    res["CheckInTime"] = this->CheckInTime->ToStringDateTime();
    res["CheckOutTime"] = this->CheckOutTime->ToStringDateTime();
    return res;
}


EmployeeRepositoryInfo::EmployeeRepositoryInfo() {
    this->m_FileHandler = std::make_unique<JsonHandle>();
}

std::vector<std::shared_ptr<Employee>> EmployeeRepositoryInfo::GetAllEmployeeInfo(){
    std::vector<std::shared_ptr<Employee>> EmployeeList;
    this->m_FileHandler->LoadFile("data\\EmployeeInfo.json");
    // this->m_FileHandler->LoadFile("/home/hlt/Documents/Restaurant-Management-system/data/EmployeeInfo.json");

    json Data = this->m_FileHandler->GetDaTa();
    for (auto it = Data.begin(); it != Data.end(); it++) {
        std::shared_ptr<Employee> tmp = std::make_shared<Employee>
        (
            (*it)["Name"],
            (*it)["Email"],
            (*it)["Phone"],
            Person::StringToGender((*it)["Gender"]),
            DateTime::FromDateString((*it)["DateJoined"]),
            (*it)["Salary"],
            Employee::StringToPosition((*it)["Position"])
        );
        tmp->SetEmployeeID((*it)["ID"]);
        EmployeeList.push_back(tmp);
    }
    LOG_INFO("Successfully loaded {} employees infomation from {}", std::to_string(EmployeeList.size()),
             this->m_FileHandler->GetFilePath());
    return EmployeeList;
}

void EmployeeRepositoryInfo::SaveEmployeeInfo(std::vector<std::shared_ptr<Employee> > &EmployeeInfo) {
    json data = json::array();
    for (auto it: EmployeeInfo)
        data.push_back(it->EmployeeToJson());
    std::ofstream file(m_FileHandler->GetFilePath());
    if (m_FileHandler->GetFilePath() == "" || !file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + m_FileHandler->GetFilePath());
    }
    file << data.dump(4);
    LOG_INFO("Saved {} Employees Infomation to {}", EmployeeInfo.size(), m_FileHandler->GetFilePath());
}

JsonHandle &EmployeeRepositoryInfo::GetFileHandler() {
    return *m_FileHandler;
}


EmployeeRepositoryAttendance::EmployeeRepositoryAttendance() {
    this->m_FileHandler = std::make_unique<JsonHandle>();
}

std::vector<std::shared_ptr<AttendanceRecord> > EmployeeRepositoryAttendance::GetAllEmployeeAttendanceRecord() {
    std::vector<std::shared_ptr<AttendanceRecord> > record;
    this->m_FileHandler->LoadFile("data\\AttendanceRecord.json");
    json Data = this->m_FileHandler->GetDaTa();
    for (auto it = Data.begin(); it != Data.end(); it++) {
        int ID = (*it)["ID"];
        std::shared_ptr tmp = std::make_shared<AttendanceRecord>(ID);
        tmp->CheckIn(DateTime::FromDateTimeString((*it)["CheckInTime"]));
        tmp->CheckOut(DateTime::FromDateTimeString((*it)["CheckOutTime"]));
        record.push_back(tmp);
    }
    return record;
}

void EmployeeRepositoryAttendance::SaveEmployeeAttendaceRecord(
    std::vector<std::shared_ptr<AttendanceRecord> > &record) {
    json data = json::array();
    for (auto it: record)
        data.push_back(it->ToJson());
    std::ofstream file(m_FileHandler->GetFilePath());
    if (m_FileHandler->GetFilePath() == "" || !file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + m_FileHandler->GetFilePath());
    }
    file << data.dump(4);
    LOG_INFO("Saved {} Employees Attendance Record to {}", record.size(), m_FileHandler->GetFilePath());
}

JsonHandle &EmployeeRepositoryAttendance::GetFileHandler() {
    return *m_FileHandler;
}

