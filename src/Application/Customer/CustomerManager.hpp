#pragma once
#include"../../Domain/Service/Customer/CustomerService.hpp"
class CustomerManager{
public:
    static CustomerManager& GetInstance();

    void AddCustomerInfo(std::shared_ptr<Customer> CustomerInfo);
    void RemoveCustomerByPhone(const std::string& Phone);
    std::vector<std::shared_ptr<Customer>> GetAllCustomerInfo();
    std::shared_ptr<Customer> SearchCustomerByPhone(const std::string& Phone);
    std::vector<std::shared_ptr<Customer>> SearchCustomerByName(const std::string&  Name);
    void SaveCustomerInfo();

private:
    CustomerManager();
    std::unique_ptr<CustomerInfoService> m_Info;
};