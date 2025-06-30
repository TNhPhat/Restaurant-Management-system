#pragma once
#include"../../Domain/Entity/Employee/Employee.hpp"
#include"FileHandle.hpp"
#include"IDManager.hpp"
#include"Logger.hpp"
class EmployeeManager{
public:
    ~EmployeeManager();
    static EmployeeManager &GetInstance();
    static void Init(const std::string& FilePath, JsonHandle* FileHandler);
    void SaveEmployeeInfo();
    void AddEmployee(const Employee& employee);
    Employee& SearchEmployeebyId(const int& ID);
    Employee& SearchEmployeebyName(const std::string& Name);
    void RemoveEmployeebyId(const int& ID);

private:

    EmployeeManager(const std::string& FilePath, JsonHandle* FileHandler);
    static std::unique_ptr<EmployeeManager> s_Instance;

    JsonHandle* m_FileHandler;

    void LoadEmployeeInfo();
    int m_EmployeeCount;
    std::vector<std::shared_ptr<Employee>> m_EmployeeList;
};