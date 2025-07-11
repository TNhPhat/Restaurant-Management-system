#pragma once 

enum class BillType {
    Income,
    Expense
};

inline std::string BillTypeToString(BillType type) {
    return (type == BillType::Income) ? "Income" : "Expense";
}

inline BillType StringToBillType(const std::string& typeStr) {
    if (typeStr == "income" || typeStr == "Income")
        return BillType::Income;
    if (typeStr == "expense" || typeStr == "Expense")
        return BillType::Expense;
    return BillType::Income; // Default fallback
}