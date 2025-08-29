#include "Dashboard.hpp"

#include "IDManager.hpp"
#include "Application/Storage/StorageManager.hpp"
#include "BudgetScreen.hpp"
#include "MenuScreen.hpp"
#include "OrderScreen.hpp"
#include "ReservationScreen.hpp"
#include "StorageScreen.hpp"
#include "EmployeesScreen.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

#include "FileMenuRepository.hpp"
#include "UI/UICore/Core.hpp"

Dashboard::Dashboard(Core &core): Screen(core) {
}

void Dashboard::Render(float DT) {
    // Title
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]); // Assuming default font, you may want to load a larger font
    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize("Restaurant Management System").x) * 0.5f);
    ImGui::Text("Restaurant Management System");
    ImGui::PopFont();

    ImGui::Separator();
    ImGui::Spacing();

    // Current time and date
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%A, %B %d, %Y - %I:%M:%S %p");

    ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(ss.str().c_str()).x) * 0.5f);
    ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "%s", ss.str().c_str());

    ImGui::Spacing();
    ImGui::Spacing();

    // Navigation buttons
    struct NavButton {
        const char *label;
        ImVec4 color;
        std::function<void()> action;
    };

    std::vector<NavButton> nav_buttons = {
        {"Orders", ImVec4(0.2f, 0.7f, 0.2f, 1.0f), [this]() { NavigateToOrderScreen(); }},
        {"Menu", ImVec4(0.7f, 0.5f, 0.2f, 1.0f), [this]() { NavigateToMenuScreen(); }},
        {"Budget", ImVec4(0.2f, 0.5f, 0.7f, 1.0f), [this]() { NavigateToBudgetScreen(); }},
        {"Reservations", ImVec4(0.7f, 0.2f, 0.5f, 1.0f), [this]() { NavigateToReservationScreen(); }},
        {"Storage", ImVec4(0.5f, 0.3f, 0.7f, 1.0f), [this]() { NavigateToStorageScreen(); }},
        {"Employees", ImVec4(0.5f, 0.3f, 0.7f, 1.0f), [this]() { NavigateToEmployeesScreen(); }}
    };

    // Navigation Grid - evenly distributed buttons
    ImVec2 button_size = ImVec2(200, 80);
    float window_width = ImGui::GetContentRegionAvail().x;

    // Calculate how many buttons can fit in one row
    int buttons_per_row = (int) ((window_width) / (button_size.x + 20.0f));

    // Ensure at least 1 button per row, and don't exceed total buttons
    if (buttons_per_row < 1) buttons_per_row = 1;
    if (buttons_per_row > nav_buttons.size()) buttons_per_row = nav_buttons.size();

    // Calculate total width needed for buttons
    float total_button_width = buttons_per_row * button_size.x;

    // Calculate remaining space for gaps
    float remaining_width = window_width - total_button_width;

    // Distribute remaining space evenly (gaps before, between, and after buttons)
    float gap_size = remaining_width / (buttons_per_row + 1);

    // Render buttons in rows with even distribution
    for (int i = 0; i < nav_buttons.size(); ++i) {
        // Start new row
        if (i % buttons_per_row == 0) {
            if (i > 0) ImGui::NewLine();
            ImGui::SetCursorPosX(gap_size); // First button starts with gap_size margin
        } else {
            // Same line with even spacing
            ImGui::SameLine(0, gap_size);
        }

        ImGui::PushStyleColor(ImGuiCol_Button, nav_buttons[i].color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(
                                  nav_buttons[i].color.x * 1.2f,
                                  nav_buttons[i].color.y * 1.2f,
                                  nav_buttons[i].color.z * 1.2f,
                                  nav_buttons[i].color.w
                              ));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(
                                  nav_buttons[i].color.x * 0.8f,
                                  nav_buttons[i].color.y * 0.8f,
                                  nav_buttons[i].color.z * 0.8f,
                                  nav_buttons[i].color.w
                              ));

        if (ImGui::Button(nav_buttons[i].label, button_size)) {
            nav_buttons[i].action();
        }

        ImGui::PopStyleColor(3);
    }

    ImGui::Spacing();
    ImGui::Spacing();

    // Quick Stats Section (if data is available)
    ImGui::Separator();
    ImGui::Text("Quick Overview");
    ImGui::Spacing();

    // Create a simple stats layout
    ImGui::Columns(4, nullptr, false);

    ImGui::TextColored(ImVec4(0.2f, 0.7f, 0.2f, 1.0f), "Orders");
    DateTime fromDT = DateTime::Now().SetHour(0).SetMinute(0).SetSecond(0), toDT = DateTime::Now().SetHour(23).
            SetMinute(59).SetSecond(59), future = DateTime(1, 1, 2100);
    ImGui::Text("Today: %d", m_OrderManager->GetOrdersInDateRange(fromDT, toDT).size());
    ImGui::Text("All time: %d", m_OrderManager->GetAllOrders().size());

    ImGui::NextColumn();
    ImGui::TextColored(ImVec4(0.7f, 0.2f, 0.5f, 1.0f), "Reservations");
    ImGui::Text("Today: %d", m_ReservationManager->GetReservationsInRange(fromDT, toDT).size());
    ImGui::Text("Upcoming: %d", m_ReservationManager->GetReservationsInRange(DateTime::Now(), future).size());

    ImGui::NextColumn();
    ImGui::TextColored(ImVec4(0.2f, 0.5f, 0.7f, 1.0f), "Revenue");
    ImGui::Text("Month: %s", CurrencyUtils::FormatUSD(m_BudgetManager->GetIncomeInMonth(DateTime::Now().GetYear(),
                    DateTime::Now().GetMonth())).c_str());
    ImGui::Text("Year: %s",
                CurrencyUtils::FormatUSD(m_BudgetManager->GetIncomeInYear(DateTime::Now().GetYear())).c_str());
    ImGui::Text("All time: %s",
                CurrencyUtils::FormatUSD(m_BudgetManager->GetIncomeTotal()).c_str());

    ImGui::NextColumn();
    ImGui::TextColored(ImVec4(0.5f, 0.3f, 0.7f, 1.0f), "Expenses");
    ImGui::Text("This Month: %s", CurrencyUtils::FormatUSD(m_BudgetManager->GetExpenseInMonth(DateTime::Now().GetYear(),
                    DateTime::Now().GetMonth())).c_str());
    ImGui::Text("This Year: %s", CurrencyUtils::FormatUSD(
                    m_BudgetManager->GetExpenseInYear(DateTime::Now().GetYear())).c_str());
    ImGui::Text("All Time: %s", CurrencyUtils::FormatUSD(m_BudgetManager->GetExpenseTotal()).c_str());

    ImGui::Columns(1);

    // Charts Section - prettier layout with proper labels
    ImGui::Separator();
    ImGui::Spacing();
    ImGui::Text("Financial Charts");
    ImGui::Spacing();

    ImGui::Columns(3, "ChartsColumns", true);
    std::vector<MonthlySummary> monthlySummaries = m_BudgetManager->GetYearlyMonthlySummary(DateTime::Now().GetYear());

    // Income Chart
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.1f, 0.8f, 0.1f, 1.0f)); // Brighter green
    ImGui::PlotHistogram("##MonthlyIncome", [](void *data, int idx) {
                             auto &summaries = *static_cast<std::vector<MonthlySummary> *>(data);
                             if (idx < 0 || idx >= summaries.size()) return 0.0f;
                             return static_cast<float>(summaries[idx].totalIncome);
                         }, &monthlySummaries, monthlySummaries.size(), 0, nullptr, 0.0f,
                         static_cast<float>(std::max_element(monthlySummaries.begin(), monthlySummaries.end(),
                                                             [](const MonthlySummary &a, const MonthlySummary &b) {
                                                                 return a.totalIncome < b.totalIncome;
                                                             })->totalIncome * 1.2), ImVec2(0, 180));
    ImGui::PopStyleColor();

    // Center the label properly within the column
    float column_width = ImGui::GetColumnWidth();
    float text_width = ImGui::CalcTextSize("Monthly Income").x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (column_width - text_width) * 0.3f);
    ImGui::TextColored(ImVec4(0.1f, 0.8f, 0.1f, 1.0f), "Monthly Income"); // Matching bright green

    ImGui::NextColumn();

    // Expense Chart
    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.9f, 0.3f, 0.3f, 1.0f)); // Brighter red
    ImGui::PlotHistogram("##MonthlyExpense", [](void *data, int idx) {
                             auto &summaries = *static_cast<std::vector<MonthlySummary> *>(data);
                             if (idx < 0 || idx >= summaries.size()) return 0.0f;
                             return static_cast<float>(summaries[idx].totalExpenses);
                         }, &monthlySummaries, monthlySummaries.size(), 0, nullptr, 0.0f,
                         static_cast<float>(std::max_element(monthlySummaries.begin(), monthlySummaries.end(),
                                                             [](const MonthlySummary &a, const MonthlySummary &b) {
                                                                 return a.totalExpenses < b.totalExpenses;
                                                             })->totalExpenses * 1.2), ImVec2(0, 180));
    ImGui::PopStyleColor();

    // Center the label properly within the column
    column_width = ImGui::GetColumnWidth();
    text_width = ImGui::CalcTextSize("Monthly Expense").x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (column_width - text_width) * 0.3f);
    ImGui::TextColored(ImVec4(0.9f, 0.3f, 0.3f, 1.0f), "Monthly Expense"); // Matching bright red

    // Tax Chart
    float taxes[12] = {};
    for (int i = 0; i < 12; i++) {
        taxes[i] = m_BudgetManager->CalculateTaxForMonth(DateTime::Now().GetYear(), i + 1).taxOwed;
    }
    ImGui::NextColumn();

    ImGui::PushStyleColor(ImGuiCol_PlotHistogram, ImVec4(0.2f, 0.5f, 1.0f, 1.0f)); // Brighter blue
    ImGui::PlotHistogram("##MonthlyTax", taxes, 12, 0, nullptr, 0.0f,
                         static_cast<float>(*std::max_element(taxes, taxes + 12) * 1.2), ImVec2(0, 180));
    ImGui::PopStyleColor();

    // Center the label properly within the column
    column_width = ImGui::GetColumnWidth();
    text_width = ImGui::CalcTextSize("Monthly Tax").x;
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (column_width - text_width) * 0.3f);
    ImGui::TextColored(ImVec4(0.2f, 0.5f, 1.0f, 1.0f), "Monthly Tax"); // Matching bright blue

    ImGui::Columns(1, nullptr, false);
    // Footer
    ImGui::SetCursorPosY(ImGui::GetWindowSize().y - 35);
    ImGui::Separator();
    ImGui::SetCursorPosX(
        (ImGui::GetWindowSize().x - ImGui::CalcTextSize("Restaurant Management System v1.0").x) * 0.5f);
    ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "Restaurant Management System v1.0");
}

void Dashboard::Init() {
    IDManager::Init("Data/IDRegistry.json");
    StorageManager::GetInstance().SetFilePath("Data/Storages.json");
    StorageManager::GetInstance().LoadStorageFromFile();
    m_MenuRepo = std::make_unique<FileMenuRepository>("Data/Menu.json", "Data/MenuItem.json", "Data/MenuAddons.json");
    m_MealRepo = std::make_shared<FileMealRepository>("Data/MealHistory.json", *m_MenuRepo);
    auto OrderRepo = std::make_unique<OrderRepository>("Data/Order.json", m_MealRepo);
    auto OrderServ = std::make_unique<OrderService>();
    m_OrderManager = std::make_unique<OrderManager>(OrderRepo, OrderServ);
    auto FileRevRepo = std::make_unique<FileReservationRepository>("Data/Reservation.json");
    m_ReservationManager = std::make_unique<ReservationManager>(FileRevRepo);
    auto budget = std::make_shared<Budget>();
    auto repository = std::make_unique<BudgetRepository>("data/Budget.json");
    auto service = std::make_unique<BudgetService>();
    m_BudgetManager = std::make_unique<BudgetManager>(budget, repository, service);
}

void Dashboard::OnExit() {
    m_OrderManager.reset();
    m_MealRepo.reset();
    m_MenuRepo.reset();
    m_ReservationManager.reset();
    m_BudgetManager.reset();
    StorageManager::GetInstance().SaveStorageToFile();
    IDManager::GetInstance().SaveID();
}

void Dashboard::NavigateToOrderScreen() {
    try {
        m_Core.PushScreen<OrderScreen>(m_Core, m_OrderManager, m_MenuRepo, m_MealRepo);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to open Order Screen: %s", e.what());
    }
}

void Dashboard::NavigateToMenuScreen() {
    try {
        m_Core.PushScreen<MenuScreen>(m_Core);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to open Menu Screen: %s", e.what());
    }
}

void Dashboard::NavigateToBudgetScreen() {
    try {
        m_Core.PushScreen<BudgetScreen>(m_Core, m_BudgetManager);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to open Budget Screen: %s", e.what());
    }
}

void Dashboard::NavigateToReservationScreen() {
    try {
        m_Core.PushScreen<ReservationScreen>(m_Core, m_ReservationManager);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to open Reservation Screen: %s", e.what());
    }
}

void Dashboard::NavigateToStorageScreen() {
    try {
        m_Core.PushScreen<StorageScreen>(m_Core);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to open Storage Screen: %s", e.what());
    }
}

void Dashboard::NavigateToEmployeesScreen() {
    try {
        m_Core.PushScreen<EmployeesScreen>(m_Core);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to open Employees Screen: %s", e.what());
    }
}
