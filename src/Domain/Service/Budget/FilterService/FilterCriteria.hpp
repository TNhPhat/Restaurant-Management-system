#pragma once

#include "DateTime.hpp"
#include "BillType.hpp"
#include <optional>

// Filter criteria structures
struct AmountRange {
    double minAmount;
    double maxAmount;
    
    AmountRange(double min, double max) : minAmount(min), maxAmount(max) {}
};

struct DateRange {
    DateTime startDate;
    DateTime endDate;
    
    DateRange(const DateTime& start, const DateTime& end) : startDate(start), endDate(end) {}
};

// Filter criteria container
class FilterCriteria {
private:
    std::optional<AmountRange> m_AmountRange;
    std::optional<DateRange> m_DateRange;
    std::optional<BillType> m_BillType;

public:
    void SetAmountRange(double min, double max) {
        m_AmountRange = AmountRange(min, max);
    }
    
    void SetDateRange(const DateTime& start, const DateTime& end) {
        m_DateRange = DateRange(start, end);
    }
    
    void SetBillType(BillType type) {
        m_BillType = type;
    }
    
    void ClearAmountRange() { m_AmountRange.reset(); }
    void ClearDateRange() { m_DateRange.reset(); }
    void ClearBillType() { m_BillType.reset(); }
    void ClearAll() {
        m_AmountRange.reset();
        m_DateRange.reset();
        m_BillType.reset();
    }
    
    // Getters
    const std::optional<AmountRange>& GetAmountRange() const { return m_AmountRange; }
    const std::optional<DateRange>& GetDateRange() const { return m_DateRange; }
    const std::optional<BillType>& GetBillType() const { return m_BillType; }
};