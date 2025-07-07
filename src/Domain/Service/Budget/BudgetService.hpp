#pragma once

#include "Bill.hpp"
#include "IDManager.hpp"


class BudgetService {
public: 
    //std::shared_ptr<Bill> GenerateBillFromOrder(const Order &UpdateOrder, const std::string &Message);
    std::shared_ptr<Bill> GenerateBill(const DateTime &Date, const std::string &Message, const double &Price, const BillType &Type);
};