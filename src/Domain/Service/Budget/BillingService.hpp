#pragma once

#include "Bill.hpp"
#include "IDManager.hpp"

class Order;

class BillingService {
public: 
    Bill GenerateBillFromOrder(const Order &UpdateOrder, const std::string &Message);
};