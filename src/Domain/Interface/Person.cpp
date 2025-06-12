#include "person.hpp"
Person::Person() : m_Name(""), m_Email(""), m_Phone("") {};
Person::Person(const std::string& Name, const std::string& Email,const std::string& Phone):
    m_Name(Name),m_Email(Email),m_Phone(Phone) {}

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