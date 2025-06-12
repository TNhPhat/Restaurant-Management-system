#pragma once
#include <string>
#include <iostream>
class Person {
public:
    Person();
    Person(const std::string &Name, const std::string &Email,const std::string &Phone);
    ~Person() = default;
    std::string GetName() const;
    std::string GetEmail() const;
    std::string GetPhone() const;
    void SetName(const std::string& Name);
    void SetEmail(const std::string& Email);
    void SetPhone(const std::string& Phone);

private:
    std::string m_Name;
    std::string m_Email;
    std::string m_Phone;
};