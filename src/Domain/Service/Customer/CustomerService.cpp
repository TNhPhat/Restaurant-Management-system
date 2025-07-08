#include"CustomerService.hpp"
CustomerInfoService::CustomerInfoService(){ 
    this->m_repo = std::make_unique<CustomerRepository>();
    this->m_Info = this->m_repo->GetAllCustomerInfo();
}

std::shared_ptr<Customer> CustomerInfoService::SearchCustomerByPhone(const std::string& Phone){
    for(auto it: m_Info)
        if(it->GetPhone() == Phone)
           return it;
    return nullptr;
}

std::vector<std::shared_ptr<Customer>> CustomerInfoService::SearchCustomerByName(const std::string& Name){
    std::vector<std::shared_ptr<Customer>> res;
    for(auto it: m_Info)
        if(it->GetName() == Name)
            res.push_back(it);
    return res;
}

std::vector<std::shared_ptr<Customer>> CustomerInfoService::GetAllCustomerInfo(){
    return this->m_Info;
}

void CustomerInfoService::AddCustomerInfo(std::shared_ptr<Customer> CustomerInfo){
    if(this->SearchCustomerByPhone(CustomerInfo->GetPhone())){
        LOG_ERROR("Phone {} is already exist can not add customer {}",CustomerInfo->GetPhone(),CustomerInfo->GetName());
        return;
    }
    this->m_Info.push_back(CustomerInfo);
    LOG_INFO("Succesfull add customer {} infomation",CustomerInfo->GetName());
}

void CustomerInfoService::RemoveCustomerByPhone(const std::string& Phone){
    for(auto it = this->m_Info.begin() ; it != this->m_Info.end() ;it++)
        if((*it)->GetPhone() == Phone){
            this->m_Info.erase(it);
            LOG_INFO("Succesfull remove customer with phone {}",Phone);
            return;
        }
}

void CustomerInfoService::SaveCustomerInfo(){
    this->m_repo->SaveCustomerInfo(this->m_Info);
}


