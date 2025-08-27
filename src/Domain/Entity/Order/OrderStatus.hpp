#pragma once
#include <algorithm>

#include <algorithm>
enum class OrderStatus {
    None,
    Received,
    Preparing,
    Complete,
    Canceled,
};

inline std::string OrderStatusToString(OrderStatus status) {
    switch (status) {
        case OrderStatus::Received:  return "Received";
        case OrderStatus::Preparing: return "Preparing";
        case OrderStatus::Complete:  return "Complete";
        case OrderStatus::Canceled:  return "Canceled";
        case OrderStatus::None:      return "None";
        default:                     return "None";
    }
}

inline OrderStatus StringToOrderStatus(const std::string& str) {
    std::string input = str;
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    if (input == "received")  return OrderStatus::Received;
    if (input == "preparing") return OrderStatus::Preparing;
    if (input == "complete")  return OrderStatus::Complete;
    if (input == "canceled")  return OrderStatus::Canceled;
    if (input == "none")      return OrderStatus::None;

    return OrderStatus::None;
}

