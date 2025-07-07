#include <string>
#include "BudgetService.hpp"
#include "Bill.hpp"

// std::shared_ptr<Bill> BudgetService::GenerateBillFromOrder(const Order &UpdateOrder, const std::string &Message) {
//     int newBillID = IDManager::GetInstance().GetNextID("bill");
//     double SubTotal = UpdateOrder.GetTotalPrice();
//     return std::make_shared<Bill>(newBillID, UpdateOrder.GetDate(), Message, UpdateOrder.GetTotalPrice(), BillType::Income);
// }

std::shared_ptr<Bill> BudgetService::GenerateBill(const DateTime &Date, const std::string &Message, const double &Price, const BillType &Type) {
    int newBillID = IDManager::GetInstance().GetNextID("Bill");
    return std::make_shared<Bill>(newBillID, Date, Message, Price, Type);
}
