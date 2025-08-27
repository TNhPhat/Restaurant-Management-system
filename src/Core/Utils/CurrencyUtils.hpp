#pragma once
#include <string>

namespace CurrencyUtils {
    // Format currency with VND symbol and thousand separators
    std::string FormatUSD(double amount, bool showSymbol = true);

    // Format currency with custom currency symbol
    std::string FormatCurrency(double amount, const std::string &symbol = "VND", bool showSymbol = true);

    // Add thousand separators to a number (e.g., 1234567 -> "1,234,567")
    std::string AddThousandSeparators(double number);

    // Parse currency string back to double (removes separators and symbols)
    double ParseCurrency(const std::string &currencyStr);
}
