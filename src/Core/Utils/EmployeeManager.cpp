#include "EmployeeManager.hpp"
std::unique_ptr<EmployeeManager> EmployeeManager::s_Instance = nullptr;
EmployeeManager::EmployeeManager(const std::string& FilePath, JsonHandle* FileHandler){
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
        json EmployeeInfo = it.value();
        std::shared_ptr<Employee> tmp = std::make_shared<Employee>
        (   
            EmployeeInfo["Name"],
            EmployeeInfo["Email"],
            EmployeeInfo["Phone"],
            Person::StringToGender(EmployeeInfo["Gender"]),
            DateTime::FromDateString(EmployeeInfo["DateJoined"]),
            EmployeeInfo["Salary"],
            Employee::StringToPosition(EmployeeInfo["Position"])
        );
        tmp->SetEmployeeID(EmployeeInfo["ID"]);
        this->m_EmployeeList.push_back(tmp);
    }
}
void EmployeeManager::AddEmployee(const Employee& employee,const IDManager& IDManager){
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
    LOG_ERROR("No Employee with ID", ID);
}

Employee& EmployeeManager::SearchEmployeebyName(const std::string& Name){
    for(auto it: this->m_EmployeeList)
        if(it->GetName() == Name)
            return *it;
    LOG_ERROR("No Employee with Name", Name);
}

void EmployeeManager::RemoveEmployeebyId(const int& ID){
    this->m_FileHandler->ExecuteCommand(new RemoveDataCommand({Employee::NumToString(ID)}));
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
    for(auto it :this->m_EmployeeList)
        this->m_FileHandler->ExecuteCommand(new AddDataCommand({},it->EmployeeToJson()));

    m_FileHandler->SaveFile();
}
EmployeeManager::~EmployeeManager(){
    delete m_FileHandler;
}