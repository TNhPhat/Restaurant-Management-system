#pragma once
#include "BudgetManager.hpp"
#include "DateTimePicker.hpp"
#include "Screen.hpp"

class BudgetScreen : public Screen {
    std::shared_ptr<BudgetManager> s_budgetManager = nullptr;
    bool m_shouldRefresh = true;
    int m_CurrentChoiceID = -1;
    int m_SelectedID = -1;
    std::string m_nameField;
    std::string m_descriptionField;
    double m_priceField = 0.0;
    bool m_typeField = false;
    std::vector<std::shared_ptr<Bill> > m_currentBills;
    DateTime m_dateField = DateTime::Now();
    DateTimePicker m_datePicker;
    ImGuiTextFilter m_filter;
    bool m_showIncome = true;
    bool m_showExpense = true;
    double m_minAmount = 0.0;
    double m_maxAmount = std::numeric_limits<double>::max();
    DateTime m_fromDate = DateTime();
    DateTime m_toDate = DateTime::Now();
    FilterCriteria m_filterCriteria;

public:
    BudgetScreen(Core &core, std::shared_ptr<BudgetManager> &budgetManager);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;
};
