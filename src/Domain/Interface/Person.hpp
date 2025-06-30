#pragma once
#include <string>
#include <iostream>

enum Gender{
    Female,
    Male
};

class Person {
public:
    Person();
    Person(const std::string &Name, const std::string &Email,const std::string &Phone,const Gender& Gender);
    ~Person() = default;
    std::string GetName() const;
    std::string GetEmail() const;
    std::string GetPhone() const;
    Gender GetGender() const;
    void SetName(const std::string& Name);
    void SetEmail(const std::string& Email);
    void SetPhone(const std::string& Phone);
    void SetGender(const Gender& Gender);
    std::string GenderToString() const;
    static Gender StringToGender(const std::string& gender);

private:
    std::string m_Name;
    std::string m_Email;
    std::string m_Phone;
    Gender m_Gender;
};