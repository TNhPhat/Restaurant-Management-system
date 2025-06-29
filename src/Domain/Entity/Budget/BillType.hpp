#pragma once 

enum class BillType {
    Income,
    Expense
};

inline std::string BillTypeToString(BillType type) {
    return (type == BillType::Income) ? "Income" : "Expense";
}

inline BillType StringToBillType(const std::string& str) {
    return (str == "Expense") ? BillType::Expense : BillType::Income;
}