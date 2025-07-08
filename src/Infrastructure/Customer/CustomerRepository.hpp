#pragma once
#include<iostream>
#include"../../Domain/Entity/Customer/Customer.hpp"
class CustomerRepository{
public:
    ~CustomerRepository() = default;
    CustomerRepository();
    std::vector<std::shared_ptr<Customer>> GetAllCustomerInfo();
    void SaveCustomerInfo(std::vector<std::shared_ptr<Customer>>& CustomerInfo);
private:
    std::unique_ptr<JsonHandle> m_FileHandler;
};