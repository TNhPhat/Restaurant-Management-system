#include "Person.hpp"
Person::Person() : m_Name(""), m_Email(""), m_Phone("") {};

Person::Person(const std::string& Name, const std::string& Email,const std::string& Phone,const Gender& Gender):
    m_Name(Name),m_Email(Email),m_Phone(Phone),m_Gender(Gender) {}

std::string Person::GetName() const {
    return m_Name;
}

std::string Person::GetEmail() const {
    return m_Email;
}

std::string Person::GetPhone() const {
    return m_Phone;
}

void Person::SetName(const std::string& Name) {
    m_Name = Name;
}

void Person::SetEmail(const std::string& Email) {
    m_Email = Email;
}

void Person::SetPhone(const std::string& Phone) {
    m_Phone = Phone;
}

void Person::SetGender(const Gender& Gender){
    this->m_Gender = Gender;
}

std::string Person::GenderToString() const{
    switch (this->m_Gender)
    {
        case Gender::Female:
            return "Female";
        case Gender::Male:
            return "Male";
        default:
            return "";
    }
}

Gender Person::StringToGender(const std::string& gender){
    if(gender == "Male")
        return Gender::Male;
    else    
        return Gender::Female;
}