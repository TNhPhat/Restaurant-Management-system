#pragma once
#include"../../../Infrastructure/Customer/CustomerRepository.hpp"
class CustomerInfoService{
public: 
    CustomerInfoService();
    void AddCustomerInfo(std::shared_ptr<Customer> CustomerInfo);
    void RemoveCustomerByPhone(const std::string& Phone);
    std::shared_ptr<Customer> SearchCustomerByPhone(const std::string& Phone);
    ~CustomerInfoService() = default;
    std::vector<std::shared_ptr<Customer>> SearchCustomerByName(const std::string& Name);
    std::vector<std::shared_ptr<Customer>> GetAllCustomerInfo();
    void SaveCustomerInfo();
private:
    std::unique_ptr<CustomerRepository> m_repo;
    std::vector<std::shared_ptr<Customer>> m_Info;
};