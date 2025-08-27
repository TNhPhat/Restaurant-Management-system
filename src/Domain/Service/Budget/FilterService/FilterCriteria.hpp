#pragma once

#include "DateTime.hpp"
#include "BillType.hpp"
#include <optional>

// Filter criteria structures
struct AmountRange {
    double minAmount;
    double maxAmount;

    AmountRange(double min, double max) : minAmount(min), maxAmount(max) {
    }
};

struct DateRange {
    DateTime startDate;
    DateTime endDate;

    DateRange(const DateTime &start, const DateTime &end) : startDate(start), endDate(end) {
    }
};

// Filter criteria container
class FilterCriteria {
private:
    std::optional<AmountRange> m_AmountRange;
    std::optional<DateRange> m_DateRange;
    bool m_ShowIncome = true;
    bool m_ShowExpense = true;

public:
    void SetAmountRange(double min, double max) {
        m_AmountRange = AmountRange(min, max);
    }

    void SetDateRange(const DateTime &start, const DateTime &end) {
        m_DateRange = DateRange(start, end);
    }

    void SetBillType(BillType type) {
        if (type == BillType::Income) {
            m_ShowIncome = true;
            m_ShowExpense = false;
        } else {
            m_ShowIncome = false;
            m_ShowExpense = true;
        }
    }

    void SetShowIncome(bool show) { m_ShowIncome = show; }
    void SetShowExpense(bool show) { m_ShowExpense = show; }

    void ClearAmountRange() { m_AmountRange.reset(); }
    void ClearDateRange() { m_DateRange.reset(); }

    void ClearBillType() {
        m_ShowIncome = true;
        m_ShowExpense = true;
    }

    void ClearAll() {
        m_AmountRange.reset();
        m_DateRange.reset();
        ClearBillType();
    }

    // Getters
    const std::optional<AmountRange> &GetAmountRange() const { return m_AmountRange; }
    const std::optional<DateRange> &GetDateRange() const { return m_DateRange; }
    const bool &GetShowIncome() const { return m_ShowIncome; }
    const bool &GetShowExpense() const { return m_ShowExpense; }
};
