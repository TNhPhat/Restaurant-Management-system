#include"CustomerRepository.hpp"
CustomerRepository::CustomerRepository(){
    this->m_FileHandler = std::make_unique<JsonHandle>();
}

std::vector<std::shared_ptr<Customer>> CustomerRepository::GetAllCustomerInfo(){
    this->m_FileHandler->LoadFile("data\\CustomerInfo.json");
    std::vector<std::shared_ptr<Customer>> CustomerInfo;
    json Data = this->m_FileHandler->GetDaTa();
    for(auto it = Data.begin() ; it != Data.end() ; it++){
        std::shared_ptr<Customer> tmp = std::make_shared<Customer>
        (
            (*it)["Name"],
            (*it)["Email"],
            (*it)["Phone"],
            Person::StringToGender((*it)["Gender"])
        );
        tmp->SetPoints((*it)["MemberPoints"]);
        CustomerInfo.push_back(tmp);
    }
    LOG_INFO("Successfully loaded {} Customers infomation from {}",std::to_string(CustomerInfo.size()),this->m_FileHandler->GetFilePath());
    return CustomerInfo;
}

void CustomerRepository::SaveCustomerInfo(std::vector<std::shared_ptr<Customer>>& CustomerInfo){
     json data = json::array();
    for(auto it : CustomerInfo)
        data.push_back(it->ToJson());
    std::ofstream file(this->m_FileHandler->GetFilePath());
    if (this->m_FileHandler->GetFilePath() == "" || !file.is_open())
    {
        throw std::runtime_error("Cannot open file for writing: " + m_FileHandler->GetFilePath());
    }
    file << data.dump(4);
    LOG_INFO("Saved {} Customers Infomation to {}",CustomerInfo.size(),m_FileHandler->GetFilePath());

}