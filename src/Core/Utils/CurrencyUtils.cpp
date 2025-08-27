#include "CurrencyUtils.hpp"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <locale>

namespace CurrencyUtils {
    std::string FormatUSD(double amount, bool showSymbol) {
        return FormatCurrency(amount, "$", showSymbol);
    }

    std::string FormatVND(double amount, bool showSymbol) {
        return FormatCurrency(amount, "$", showSymbol);
    }

    std::string FormatCurrency(double amount, const std::string &symbol, bool showSymbol) {
        std::ostringstream oss;

        // Handle negative amounts
        bool isNegative = amount < 0;
        if (isNegative) {
            amount = -amount;
            oss << "-";
        }

        // Format the number with thousand separators and 2 decimal places
        std::string formattedNumber = AddThousandSeparators(amount);
        oss << formattedNumber;

        // Add currency symbol if requested
        if (showSymbol) {
            oss << " " << symbol;
        }

        return oss.str();
    }

    std::string AddThousandSeparators(double number) {
        std::ostringstream oss;
        oss << std::fixed << std::setprecision(2) << number;
        std::string numStr = oss.str();

        // Find the decimal point
        size_t decimalPos = numStr.find('.');
        std::string integerPart = numStr.substr(0, decimalPos);
        std::string decimalPart = (decimalPos != std::string::npos) ? numStr.substr(decimalPos) : "";

        // Add commas for thousand separators to integer part
        std::string result;
        int count = 0;

        // Process from right to left
        for (int i = integerPart.length() - 1; i >= 0; i--) {
            if (count == 3) {
                result = "," + result;
                count = 0;
            }
            result = integerPart[i] + result;
            count++;
        }

        // Add decimal part back
        result += decimalPart;

        return result;
    }

    double ParseCurrency(const std::string &currencyStr) {
        std::string cleanStr = currencyStr;

        // Remove currency symbols and spaces
        std::string symbolsToRemove = "VND$€£¥₫ ";
        for (char c: symbolsToRemove) {
            cleanStr.erase(std::remove(cleanStr.begin(), cleanStr.end(), c), cleanStr.end());
        }

        // Remove commas (thousand separators)
        cleanStr.erase(std::remove(cleanStr.begin(), cleanStr.end(), ','), cleanStr.end());

        // Convert to double
        try {
            return std::stod(cleanStr);
        } catch (const std::exception &) {
            return 0.0; // Return 0 if parsing fails
        }
    }
}
