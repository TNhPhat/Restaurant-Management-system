#include "EmployeeManager.hpp"
std::unique_ptr<EmployeeManager> EmployeeManager::s_Instance = nullptr;
EmployeeManager::EmployeeManager(const std::string& FilePath, JsonHandle* FileHandler){
    m_EmployeeCount = 0;
    this->m_FileHandler  = FileHandler;
    this->m_FileHandler->LoadFile(FilePath);
    LoadEmployeeInfo();
}
void EmployeeManager::Init(const std::string& FilePath, JsonHandle* FileHandler){
    s_Instance = std::unique_ptr<EmployeeManager>(new EmployeeManager(FilePath,FileHandler));
}

void EmployeeManager::LoadEmployeeInfo(){
    json Data = this->m_FileHandler->GetDaTa();
    for(auto it = Data.begin() ; it != Data.end() ;it++){
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
        this->m_EmployeeList.push_back(tmp);
        m_EmployeeCount++;
    }
    LOG_INFO("Successfully loaded {} employees from {}",std::to_string(m_EmployeeCount),m_FileHandler->GetFilePath());
}
void EmployeeManager::AddEmployee(const Employee& employee){
    std::shared_ptr employeeptr = std::make_shared<Employee>
    (   
        employee.GetName(),
        employee.GetEmail(),
        employee.GetPhone(),
        employee.GetGender(),
        employee.GetDateJoined(),
        employee.GetSalary(),
        employee.GetPostion()
    );
    employeeptr->SetEmployeeID(IDManager::GetInstance().GetNextID("Employee"));
    m_EmployeeList.push_back(employeeptr);
}

Employee& EmployeeManager::SearchEmployeebyId(const int& ID){
    for(auto it: this->m_EmployeeList)
        if(it->GetEmployeeID())
            return *it;
    LOG_ERROR("No Employee with ID: {}", ID);
}

Employee& EmployeeManager::SearchEmployeebyName(const std::string& Name){
    for(auto it: this->m_EmployeeList)
        if(it->GetName() == Name)
            return *it;
    LOG_ERROR("No Employee with Name: {}", Name);
}

void EmployeeManager::RemoveEmployeebyId(const int& ID){
    for(auto it = m_EmployeeList.begin() ; it != m_EmployeeList.end() ; it++){
        if((*it)->GetEmployeeID() == ID){
            m_EmployeeList.erase(it);
            break;
        }
    }
}

EmployeeManager& EmployeeManager::GetInstance(){
    return *s_Instance;
}
void EmployeeManager::SaveEmployeeInfo(){
    json data = json::array();
    for(auto it : m_EmployeeList)
        data.push_back(it->EmployeeToJson());
    std::ofstream file(m_FileHandler->GetFilePath());
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open file for writing: " + m_FileHandler->GetFilePath());
    }
    file << data.dump(4);
    LOG_INFO("Saved Employee Infomation to {}",m_FileHandler->GetFilePath());
}
EmployeeManager::~EmployeeManager(){
    delete m_FileHandler;
}