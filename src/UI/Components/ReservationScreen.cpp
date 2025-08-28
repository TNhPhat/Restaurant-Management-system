#include "ReservationScreen.hpp"
#include "imgui.h"
#include "UI/UICore/Core.hpp"
#include <array>

ReservationScreen::ReservationScreen(Core &core, std::shared_ptr<ReservationManager> manager)
    : Screen(core), m_Manager(manager) {
    // Initialize buffers with reasonable sizes
    m_NewPhoneInput.resize(256);
}

void ReservationScreen::Init() {
    // Optional: load UI state if needed
}

void ReservationScreen::OnExit() {
    // Optional cleanup
}

void ReservationScreen::Render(float dt) {
    ImGui::Text("Restaurant Reservation Management");
    ImGui::Separator();
    
    DrawBackButton();
    DrawNewReservationInput();
    ImGui::Spacing();

    DrawReservationTable();
    ImGui::Spacing();

    DrawSaveButton();
    ImGui::SameLine();
}

// Draw input + button to add new reservation
void ReservationScreen::DrawNewReservationInput() {
    if (ImGui::InputText("New Phone Number", m_NewPhoneInput.data(), m_NewPhoneInput.size())) {
        // InputText modifies the buffer, so we need to update our string
        m_NewPhoneInputStr = std::string(m_NewPhoneInput.data());
    }

    if (ImGui::Button("Add")) {
        if (!m_NewPhoneInputStr.empty()) {
            m_Manager->AddAReservationByPhoneNumber(m_NewPhoneInputStr);
            m_NewPhoneInputStr.clear();
            std::fill(m_NewPhoneInput.begin(), m_NewPhoneInput.end(), '\0');
        }
    }
}

// Draw table showing all reservations and allow edit/delete
void ReservationScreen::DrawReservationTable() {
    const auto reservations = m_Manager->GetAllReservations();

    if (ImGui::BeginTable("Reservations", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Phone");
        ImGui::TableSetupColumn("People");
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Check-in", ImGuiTableColumnFlags_WidthStretch, 2.0f);
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (const auto &r: reservations) {
            const std::string &phone = r->getPhoneNumber();
            ImGui::TableNextRow();

            // --- Phone (non-editable) ---
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", phone.c_str());

            // --- PeopleCount ---
            ImGui::TableSetColumnIndex(1);
            int &people = m_EditedPeopleCount[phone];
            if (people == 0) people = r->getPeopleCount(); // default
            ImGui::InputInt(("##people" + phone).c_str(), &people);

            // --- Reservation Time (read-only) ---
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", r->getTimeOfReservation().ToStringDateTime().c_str());

            // --- Check-in Time (dropdowns) ---
            ImGui::TableSetColumnIndex(3);

            // Initialize editor state
            if (m_EditedDateTime.find(phone) == m_EditedDateTime.end()) {
                DateTime init = r->getCheckinTime().GetYear() > 0
                                    ? r->getCheckinTime()
                                    : DateTime::Now();
                m_EditedDateTime[phone] = init;
            }
            DateTime &dt = m_EditedDateTime[phone];

            // Valid values
            auto days = DateTime::GetValidDays(dt.GetMonth(), dt.GetYear());
            auto months = DateTime::GetValidMonths();
            auto hours = DateTime::GetValidHours();
            auto minutes = DateTime::GetValidMinutes(5);

            int currentYear = DateTime::Now().GetYear();
            std::vector<int> years = {currentYear, currentYear + 1};

            // --- Day combo ---
            {
                std::string id = "##day_" + phone;
                ImGui::SetNextItemWidth(100);
                if (ImGui::BeginCombo(id.c_str(), std::to_string(dt.GetDay()).c_str())) {
                    for (int d: days) {
                        bool selected = (d == dt.GetDay());
                        if (ImGui::Selectable(std::to_string(d).c_str(), selected)) {
                            dt.SetDay(d);
                        }
                        if (selected) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::SameLine();

            // --- Month combo ---
            {
                std::string id = "##month_" + phone;
                ImGui::SetNextItemWidth(100);
                if (ImGui::BeginCombo(id.c_str(), DateTime::MonthToString(dt.GetMonth()).c_str())) {
                    for (int m: months) {
                        bool selected = (m == dt.GetMonth());
                        if (ImGui::Selectable(DateTime::MonthToString(m).c_str(), selected)) {
                            dt.SetMonth(m);
                            // clamp day if needed
                            if (!DateTime::IsValidDate(dt.GetDay(), m, dt.GetYear())) {
                                dt.SetDay(std::min(dt.GetDay(), DateTime::GetDaysInMonth(m, dt.GetYear())));
                            }
                        }
                        if (selected) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::SameLine();

            // --- Year combo ---
            {
                std::string id = "##year_" + phone;
                ImGui::SetNextItemWidth(100);
                if (ImGui::BeginCombo(id.c_str(), std::to_string(dt.GetYear()).c_str())) {
                    for (int y: years) {
                        bool selected = (y == dt.GetYear());
                        if (ImGui::Selectable(std::to_string(y).c_str(), selected)) {
                            dt.SetYear(y);
                            // clamp day if needed
                            if (!DateTime::IsValidDate(dt.GetDay(), dt.GetMonth(), y)) {
                                dt.SetDay(std::min(dt.GetDay(), DateTime::GetDaysInMonth(dt.GetMonth(), y)));
                            }
                        }
                        if (selected) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::SameLine();

            // --- Hour combo ---
            {
                std::string hourLabel = (dt.GetHour() < 10 ? "0" : "") + std::to_string(dt.GetHour());
                std::string id = "##hour_" + phone;
                ImGui::SetNextItemWidth(80);
                if (ImGui::BeginCombo(id.c_str(), hourLabel.c_str())) {
                    for (int h: hours) {
                        std::string label = (h < 10 ? "0" : "") + std::to_string(h);
                        bool selected = (h == dt.GetHour());
                        if (ImGui::Selectable(label.c_str(), selected)) {
                            dt.SetHour(h);
                        }
                        if (selected) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }
            ImGui::SameLine();

            // --- Minute combo ---
            {
                std::string minuteLabel = (dt.GetMinute() < 10 ? "0" : "") + std::to_string(dt.GetMinute());
                std::string id = "##minute_" + phone;
                ImGui::SetNextItemWidth(80 );
                if (ImGui::BeginCombo(id.c_str(), minuteLabel.c_str())) {
                    for (int m: minutes) {
                        std::string label = (m < 10 ? "0" : "") + std::to_string(m);
                        bool selected = (m == dt.GetMinute());
                        if (ImGui::Selectable(label.c_str(), selected)) {
                            dt.SetMinute(m);
                        }
                        if (selected) ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
            }

            // --- Status combo ---
            ImGui::TableSetColumnIndex(4);
            ReservationStatus &status = m_EditedStatus[phone];
            if (status == ReservationStatus::Requested && r->getStatus() != ReservationStatus::Requested)
                status = r->getStatus(); // preserve

            static const std::array<const char *, 3> statusLabels = {
                "Requested", "Pending", "CheckedIn"
            };

            int statusIndex = static_cast<int>(status);
            if (ImGui::Combo(("##status" + phone).c_str(), &statusIndex,
                             statusLabels.data(), static_cast<int>(statusLabels.size()))) {
                status = static_cast<ReservationStatus>(statusIndex);
            }

            // --- Actions ---
            ImGui::TableSetColumnIndex(5);
            if (ImGui::Button(("Update##" + phone).c_str())) {
                r->setPeopleCount(people);
                r->setCheckinTime(dt);
                r->setReservationStatus(status);
            }
            ImGui::SameLine();
            if (ImGui::Button(("Delete##" + phone).c_str())) {
                m_Manager->RemoveReservation(phone);
            }
        }

        ImGui::EndTable();
    }
}

// Save to file
void ReservationScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        m_Manager->SaveReservations("Data/Reservation.json");
    }
}

// Back to previous screen
void ReservationScreen::DrawBackButton() {
    if (ImGui::Button("Go Back")) {
        m_Core.PopScreen();
    }
}

const char *ReservationScreen::StatusToString(ReservationStatus status) {
    switch (status) {
        case ReservationStatus::Requested: return "Requested";
        case ReservationStatus::Pending: return "Pending";
        case ReservationStatus::CheckedIn: return "CheckedIn";
        default: return "Unknown";
    }
}
