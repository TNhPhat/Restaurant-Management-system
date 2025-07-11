#include"CustomerManager.hpp"
CustomerManager::CustomerManager(){
    this->m_Info = std::make_unique<CustomerInfoService>();
}

std::vector<std::shared_ptr<Customer>> CustomerManager::GetAllCustomerInfo(){
    return this->m_Info->GetAllCustomerInfo();
}

std::shared_ptr<Customer> CustomerManager::SearchCustomerByPhone(const std::string& Phone){
    return this->m_Info->SearchCustomerByPhone(Phone);
}

std::vector<std::shared_ptr<Customer>> CustomerManager::SearchCustomerByName(const std::string& Name){
    return this->m_Info->SearchCustomerByName(Name);
}

void CustomerManager::SaveCustomerInfo(){
    this->m_Info->SaveCustomerInfo();
}

void CustomerManager::AddCustomerInfo(std::shared_ptr<Customer> CustomerInfo){
    this->m_Info->AddCustomerInfo(CustomerInfo);
}

void CustomerManager::RemoveCustomerByPhone(const std::string& Phone){
    this->m_Info->RemoveCustomerByPhone(Phone);
}