#pragma once
#include"../../Domain/Service/Customer/CustomerService.hpp"
class CustomerManager{
public:
    CustomerManager();
    ~CustomerManager() = default;
    
    void AddCustomerInfo(std::shared_ptr<Customer> CustomerInfo);
    void RemoveCustomerByPhone(const std::string& Phone);
    std::vector<std::shared_ptr<Customer>> GetAllCustomerInfo();
    std::shared_ptr<Customer> SearchCustomerByPhone(const std::string& Phone);
    std::vector<std::shared_ptr<Customer>> SearchCustomerByName(const std::string&  Name);
    void SaveCustomerInfo();

private:
   
    std::unique_ptr<CustomerInfoService> m_Info;
};