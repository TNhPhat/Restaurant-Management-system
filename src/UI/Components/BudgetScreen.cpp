#include "BudgetScreen.hpp"
#include "../../Core/Utils/CurrencyUtils.hpp"

#include "Constants.hpp"
#include "imgui_internal.h"
#include "UI/UICore/Core.hpp"
#include "misc/cpp/imgui_stdlib.h"

BudgetScreen::BudgetScreen(Core &core, std::shared_ptr<BudgetManager> &budget_manager): Screen(core),
    s_budgetManager(budget_manager) {
}

void BudgetScreen::Render(float DT) {
    // Make Budget title more eye-catching
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.2f, 1.0f)); // Green color
    ImGui::GetFont()->Scale *= 2.0f;
    ImGui::PushFont(ImGui::GetFont());
    ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("BUDGET MANAGEMENT").x) / 2);
    ImGui::Text("BUDGET MANAGEMENT");
    ImGui::PopFont();
    ImGui::GetFont()->Scale /= 2.0f;
    ImGui::PopStyleColor();

    if (ImGui::Button("Go Back")) {
        m_Core.PopScreen();
    }
    ImGui::Separator();

    // Enhanced budget summary with colors and icons
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 8));

    // Income (Green)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.2f, 0.8f, 0.3f, 1.0f));
    ImGui::Text("Total Income: %s", CurrencyUtils::FormatUSD(s_budgetManager->GetIncomeTotal()).c_str());
    ImGui::PopStyleColor();

    // Expenses (Red)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.3f, 0.3f, 1.0f));
    ImGui::Text("Total Expenses: %s", CurrencyUtils::FormatUSD(s_budgetManager->GetExpenseTotal()).c_str());
    ImGui::PopStyleColor();

    // Taxes (Orange/Yellow)
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.7f, 0.2f, 1.0f));
    ImGui::Text("Total Taxes: %s", CurrencyUtils::FormatUSD(s_budgetManager->CalculateTotalTax().taxOwed).c_str());
    ImGui::PopStyleColor();

    // Calculate and display net profit/loss
    double netAmount = s_budgetManager->GetIncomeTotal() - s_budgetManager->GetExpenseTotal() - s_budgetManager->
                       CalculateTotalTax().taxOwed;
    if (netAmount >= 0) {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.1f, 0.9f, 0.1f, 1.0f)); // Bright green for profit
        ImGui::Text("Net Profit: %s", CurrencyUtils::FormatUSD(netAmount).c_str());
    } else {
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.2f, 0.2f, 1.0f)); // Bright red for loss
        ImGui::Text("Net Loss: %s", CurrencyUtils::FormatUSD(-netAmount).c_str());
    }
    ImGui::PopStyleColor();

    ImGui::PopStyleVar();
    ImGui::Separator();

    // Enhanced Bills section title
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.3f, 0.7f, 0.9f, 1.0f)); // Blue color
    ImGui::Text("Bills Management:");
    ImGui::PopStyleColor();

    if (ImGui::BeginChild("BillsChild", ImVec2(200, 0), true)) {
        if (ImGui::Button("Refresh", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_shouldRefresh = true;
        }
        if (ImGui::Button("Add Bill", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            this->m_nameField = "";
            this->m_descriptionField = "";
            this->m_priceField = 0.0;
            this->m_typeField = true;
            this->m_dateField = DateTime::Now();
            this->m_datePicker.SetDateTime(m_dateField);
            this->m_datePicker.SetShowTime(true);
            this->m_datePicker.SetShowSeconds(true);
            ImGui::OpenPopup("Add Bill");
        }
        if (ImGui::BeginPopupModal("Add Bill", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Add Bill feature.");
            ImGui::Separator();
            ImGui::TextUnformatted("Message");
            ImGui::InputText("##Message", &m_nameField);
            ImGui::TextUnformatted("Amount");
            ImGui::InputDouble("USD##Amount", &m_priceField);
            ImGui::TextUnformatted("Date");
            std::string buttonText = m_dateField.ToStringDateTime();
            if (ImGui::Button(buttonText.c_str())) {
                ImGui::OpenPopup("DatePicker");
            }
            if (m_datePicker.Render("DatePicker")) {
                m_dateField = m_datePicker.GetDateTime();
            }
            ImGui::Checkbox("Is Income?", &m_typeField);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (m_typeField) {
                    s_budgetManager->AddIncome(m_dateField, m_nameField, m_priceField);
                } else {
                    s_budgetManager->AddExpense(m_dateField, m_nameField, m_priceField);
                }
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::BeginDisabled();
        }
        if (ImGui::Button("Delete Bill", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            ImGui::OpenPopup("DeleteBillPopup");
        }
        if (ImGui::BeginPopupModal("DeleteBillPopup", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto bill = s_budgetManager->FindBillByID(this->m_CurrentChoiceID);
            ImGui::Text("Are you sure you want to delete Bill ID: %d, Message: %s?",
                        bill->GetID(), bill->GetMessage().c_str());
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                s_budgetManager->RemoveBillByID(this->m_CurrentChoiceID);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::Button("Edit Bill", ImVec2(-1, Constants::BUTTON_HEIGHT))) {
            auto bill = s_budgetManager->FindBillByID(this->m_CurrentChoiceID);
            this->m_nameField = bill->GetMessage();
            this->m_priceField = bill->GetTotal();
            this->m_typeField = bill->GetType() == BillType::Income;
            this->m_dateField = bill->GetDate();
            this->m_datePicker.SetDateTime(m_dateField);
            this->m_datePicker.SetShowTime(true);
            this->m_datePicker.SetShowSeconds(true);
            ImGui::OpenPopup("Edit Bill");
        }
        if (ImGui::BeginPopupModal("Edit Bill", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto bill = s_budgetManager->FindBillByID(this->m_CurrentChoiceID);
            ImGui::Text("Edit Bill ID: %d", bill->GetID());
            ImGui::Separator();
            ImGui::TextUnformatted("Message");
            ImGui::InputText("##Message", &m_nameField);
            ImGui::TextUnformatted("Amount");
            ImGui::InputDouble("USD##Amount", &m_priceField);
            ImGui::TextUnformatted("Date");
            std::string buttonText = m_dateField.ToStringDateTime();
            if (ImGui::Button(buttonText.c_str())) {
                ImGui::OpenPopup("DatePicker");
            }
            if (m_datePicker.Render("DatePicker")) {
                m_dateField = m_datePicker.GetDateTime();
            }
            ImGui::Checkbox("Is Income?", &m_typeField);
            ImGui::Dummy(ImVec2(0.0f, 20.0f));
            if (ImGui::Button("OK", ImVec2(120, 0))) {
                bill->SetMessage(m_nameField);
                bill->SetTotal(m_priceField);
                bill->SetType(m_typeField ? BillType::Income : BillType::Expense);
                bill->SetDate(m_dateField);
                this->m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            ImGui::SetItemDefaultFocus();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (this->m_CurrentChoiceID == -1) {
            ImGui::EndDisabled();
        }
        ImGui::EndChild();
    }
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
    ImGui::SameLine();
    if (ImGui::BeginChild("BillTableChild", ImVec2(0, 0), true)) {
        ImGui::SetCursorPosX(
            ImGui::GetContentRegionAvail().x - 500 - ImGui::CalcTextSize("Filter (Ctrl+F):").x -
            ImGui::CalcTextSize("More Options").x - 20);
        ImGui::TextUnformatted("Filter (Ctrl+F):");
        ImGui::SameLine();
        ImGui::SetNextItemShortcut(ImGuiMod_Ctrl | ImGuiKey_F);
        m_filter.Draw("##Filter", 500);
        ImGui::SameLine();
        if (ImGui::Button("More Options")) {
            m_fromDate = m_filterCriteria.GetDateRange().value().startDate;
            m_toDate = m_filterCriteria.GetDateRange().value().endDate;
            m_minAmount = m_filterCriteria.GetAmountRange().value().minAmount;
            m_maxAmount = m_filterCriteria.GetAmountRange().value().maxAmount;
            m_showIncome = m_filterCriteria.GetShowIncome();
            m_showExpense = m_filterCriteria.GetShowExpense();
            ImGui::OpenPopup("Filter");
        }
        if (ImGui::BeginPopup("Filter")) {
            ImGui::TextUnformatted("Amount Range");
            ImGui::TextUnformatted("Bill Types:");
            ImGui::SetNextItemWidth(100);
            ImGui::InputDouble("Min##Amount", &m_minAmount);
            ImGui::SameLine();
            ImGui::TextUnformatted(" - ");
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::InputDouble("Max##Amount", &m_maxAmount);
            ImGui::TextUnformatted("Date Range");
            std::string fromButtonText = m_fromDate.ToStringDate();
            if (ImGui::Button(fromButtonText.c_str())) {
                m_datePicker.SetDateTime(m_fromDate);
                ImGui::OpenPopup("FromDatePicker");
            }
            if (m_datePicker.Render("FromDatePicker")) {
                m_fromDate = m_datePicker.GetDateTime();
            }
            ImGui::SameLine();
            ImGui::TextUnformatted(" - ");
            std::string toButtonText = m_toDate.ToStringDate();
            ImGui::SameLine();
            if (ImGui::Button(toButtonText.c_str())) {
                m_datePicker.SetDateTime(m_toDate);
                ImGui::OpenPopup("ToDatePicker");
            }
            if (m_datePicker.Render("ToDatePicker")) {
                m_toDate = m_datePicker.GetDateTime();
            }
            ImGui::Checkbox("Show Income", &m_showIncome);
            ImGui::SameLine();
            ImGui::Checkbox("Show Expense", &m_showExpense);
            ImGui::Dummy({0.0f, 10.0f});
            if (ImGui::Button("Apply", ImVec2(120, 0))) {
                m_filterCriteria.SetAmountRange(m_minAmount, m_maxAmount);
                m_filterCriteria.SetDateRange(m_fromDate, m_toDate);
                m_filterCriteria.SetShowIncome(m_showIncome);
                m_filterCriteria.SetShowExpense(m_showExpense);
                m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            ImGui::Spacing();
            ImGui::SameLine();
            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            ImGui::Spacing();
            ImGui::SameLine();
            if (ImGui::Button("Reset", ImVec2(120, 0))) {
                m_filterCriteria.ClearAll();
                m_filterCriteria.SetAmountRange(0.0, std::numeric_limits<double>::max());
                m_filterCriteria.SetDateRange(DateTime(), DateTime::Now());
                m_filterCriteria.SetShowIncome(true);
                m_filterCriteria.SetShowExpense(true);
                m_shouldRefresh = true;
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginTable("BillsTable", 5,
                              ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable |
                              ImGuiTableFlags_Sortable |
                              ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableSetupColumn("ID", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentDisable,
                                    50.0f);
            ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_IndentEnable,
                                    200.0f);
            ImGui::TableSetupColumn("Message", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("Amount", ImGuiTableColumnFlags_WidthFixed, 150.0f);
            ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 100.0f);
            ImGui::TableHeadersRow();

            ImGuiTableSortSpecs *sortSpecs = ImGui::TableGetSortSpecs();

            if (m_shouldRefresh || (sortSpecs && sortSpecs->SpecsDirty)) {
                m_currentBills = s_budgetManager->FilterAndSortBills(m_filterCriteria,
                                                                     static_cast<SortCriteria>(sortSpecs->Specs->
                                                                         ColumnIndex));
                if (sortSpecs && sortSpecs->Specs->SortDirection == ImGuiSortDirection_Descending) {
                    std::ranges::reverse(m_currentBills);
                }
                if (sortSpecs)
                    sortSpecs->SpecsDirty = false;
                m_shouldRefresh = false;
            }
            for (const auto &bill: m_currentBills) {
                if (m_filter.PassFilter(bill->GetMessage().c_str()) == false) {
                    continue;
                }
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                std::string id = std::to_string(bill->GetID());
                if (ImGui::Selectable(id.c_str(), this->m_CurrentChoiceID == bill->GetID(),
                                      ImGuiSelectableFlags_SpanAllColumns)) {
                    if (m_CurrentChoiceID == bill->GetID())
                        m_CurrentChoiceID = -1;
                    else
                        m_CurrentChoiceID = bill->GetID();
                }
                ImGui::TableSetColumnIndex(1);
                ImGui::Text(bill->GetDate().ToStringDateTime().c_str());
                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%s", bill->GetMessage().c_str());
                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%s", CurrencyUtils::FormatUSD(bill->GetTotal()).c_str());
                ImGui::TableSetColumnIndex(4);
                ImGui::Text("%s", bill->GetType() == BillType::Income ? "Income" : "Expense");
            }
            ImGui::EndTable();
        }
        ImGui::EndChild();
    }
}


void BudgetScreen::Init() {
    m_filterCriteria.ClearAll();
    m_filterCriteria.SetAmountRange(m_minAmount, m_maxAmount);
    m_filterCriteria.SetShowIncome(m_showIncome);
    m_filterCriteria.SetShowExpense(m_showExpense);
    m_filterCriteria.SetDateRange(m_fromDate, m_toDate);
}

void BudgetScreen::OnExit() {
}
