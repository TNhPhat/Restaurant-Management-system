#include <string>
#include "BillingService.hpp"
#include "Order.hpp"
#include "BillType.hpp"

Bill BillingService::GenerateBillFromOrder(const Order &UpdateOrder, const std::string &Message) {
    int newBillID = IDManager::GetInstance().GetNextID("bill");
    double SubTotal = UpdateOrder.GetTotalPrice();
    return Bill(newBillID, UpdateOrder.GetDate(), Message, UpdateOrder.GetTotalPrice(), BillType::Income);
}
