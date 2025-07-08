#pragma once
#include<iostream>
#include"../../Interface/Person.hpp"
#include"../../../Core/Constants.hpp"
#include"../../../Infrastructure/Logging/Logger.hpp"
#include"../../../Infrastructure/FileHandle/FileHandle.hpp"
class Customer: public Person{
public:
    Customer(const std::string &Name, const std::string &Email,const std::string &Phone,const Gender& Gender);
    ~Customer() = default;
    void EarnPointFromSpending(const int& AmountSpent);
    void RedeemPoints(const int& PointsToUse);
    int CalculateDiscountAmount(const int& PointsToUse);
    int GetPoints() const;
    void SetPoints(const int& Points);
    json ToJson() const;
private:
    int m_MemberPoints;
};