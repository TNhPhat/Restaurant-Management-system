#include"Customer.hpp"

Customer::Customer(const std::string &Name, const std::string &Email,const std::string &Phone,const Gender& Gender): Person(Name,Email,Phone,Gender){
    this->m_MemberPoints = 0;
}

void Customer::EarnPointFromSpending(const int& AmountSpent){
    if (AmountSpent <= 0){
        LOG_WARNING("The AmountSpent of Customer {} is smaller than 1",this->GetName());
        return;
    }
    this->m_MemberPoints += static_cast<int>(AmountSpent * Constants::POINTS_PER_VND);
}

void Customer::RedeemPoints(const int& PointsToUse){
    if(this->m_MemberPoints < PointsToUse){
        LOG_ERROR("PointToUse is smaller than MemberPoints of Customer {}",this->GetName());
        return;
    }
    this->m_MemberPoints -= PointsToUse;
}

int Customer::CalculateDiscountAmount(const int& PointsToUse){
    if(this->m_MemberPoints < PointsToUse){
        LOG_ERROR("PointToUse is smaller than MemberPoints of Customer {}",this->GetName());
        return 0;
    }
    return PointsToUse * Constants::VND_PER_POINT;
}

int Customer::GetPoints() const{
    return this->m_MemberPoints;
}

void Customer::SetPoints(const int& Points){
    this->m_MemberPoints = Points;
}

json Customer::ToJson() const{
    json Data;
    Data["Name"] = this->GetName();
    Data["Phone"] = this->GetPhone();
    Data["Email"] = this->GetEmail();
    Data["Gender"] = this->GenderToString();
    Data["MemberPoints"] = this->m_MemberPoints;
    return Data;
}