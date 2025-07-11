#include "OrderRepository.hpp"
#include "Logger.hpp"
#include "DateTime.hpp"

OrderRepository::OrderRepository(const std::string& FilePath, JsonHandle* FileHandler, const IMealRepository& MealRepo)
    : m_FilePath(FilePath), m_FileHandler(FileHandler), m_MealRepo(MealRepo)
{
    m_FileHandler->LoadFile(FilePath);
}

std::vector<std::shared_ptr<Order>> OrderRepository::LoadAllOrders() {
    m_FileHandler->LoadFile(m_FilePath);
    json Data = m_FileHandler->GetDaTa();

    std::vector<std::shared_ptr<Order>> Orders;

    if (!Data.contains("Orders") || !Data["Orders"].is_array()) {
        LOG_WARNING("No 'Orders' array found in file: {}", m_FilePath);
        return Orders;
    }

    for (const auto& OrderJson : Data["Orders"]) {
        int ID = OrderJson.value("ID", 0);
        int TableID = OrderJson.value("TableID", -1);
        std::string CustomerPhone = OrderJson.value("CustomerPhone", "");
        std::string DateStr = OrderJson.value("Date", "");
        std::string StatusStr = OrderJson.value("Status", "None");

        DateTime Date = DateTime::FromDateTimeString(DateStr);
        OrderStatus Status = StringToOrderStatus(StatusStr);

        auto OrderObj = std::make_shared<Order>(ID, TableID, CustomerPhone, Date);
        OrderObj->SetOrderStatus(Status);

        if (OrderJson.contains("MealIDs") && OrderJson["MealIDs"].is_array()) {
            for (const auto& MealID : OrderJson["MealIDs"]) {
                auto meal = m_MealRepo.GetMealByID(MealID.get<int>());
                if (meal) {
                    OrderObj->AddMeal(meal);
                } else {
                    LOG_WARNING("Meal ID {} not found in MealRepository", MealID.get<int>());
                }
            }
        }

        Orders.push_back(OrderObj);
    }

    return Orders;
}

void OrderRepository::SaveAllOrders(const std::vector<std::shared_ptr<Order>>& Orders) {
    json OrdersJson = json::array();

    for (const auto& Order : Orders) {
        json OrderData;
        OrderData["ID"] = Order->GetID(); // You may need to add this getter
        OrderData["TableID"] = Order->GetTableID();
        OrderData["CustomerID"] = Order->GetCustomerPhone();
        OrderData["Date"] = Order->GetDate().ToStringDateTime(); // format: dd/MM/yyyy HH:mm:ss
        OrderData["Status"] = OrderStatusToString(Order->GetStatus());

        json MealIDs = json::array();
        for (const auto& meal : Order->GetMeals()) {
            MealIDs.push_back(meal->GetID());
        }

        OrderData["MealIDs"] = MealIDs;
        OrdersJson.push_back(OrderData);
    }

    json Root;
    Root["Orders"] = OrdersJson;

    std::ofstream outFile(m_FilePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open order file: {}", m_FilePath);
        return;
    }

    outFile << Root.dump(4);
    outFile.close();
    LOG_INFO("Orders saved to file: {}", m_FilePath);
}
