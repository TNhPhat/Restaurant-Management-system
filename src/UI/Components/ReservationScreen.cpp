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
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    DrawNewReservationInput();
    ImGui::Spacing();

    DrawReservationTable();
    ImGui::Spacing();

    DrawSaveButton();
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
        ImGui::TableSetupColumn("Reservation Time", ImGuiTableColumnFlags_WidthFixed, 200.0f);
        ImGui::TableSetupColumn("Check-in Time", ImGuiTableColumnFlags_WidthFixed, 200.0f);
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
            ImGui::SetNextItemWidth(80);
            ImGui::InputInt(("##people" + phone).c_str(), &people);

            // --- Reservation Time with DateTimePicker ---
            ImGui::TableSetColumnIndex(2);

            // Initialize reservation time editor state if not exists
            if (m_EditedReservationTime.find(phone) == m_EditedReservationTime.end()) {
                m_EditedReservationTime[phone] = r->getTimeOfReservation();
            }
            DateTime &reservationTime = m_EditedReservationTime[phone];

            // DateTimePicker button for reservation time
            std::string reservationButtonText = reservationTime.ToStringDateTime();
            if (ImGui::Button((reservationButtonText + "##reservation" + phone).c_str())) {
                m_DatePicker.SetDateTime(reservationTime);
                m_DatePicker.SetShowTime(true);
                m_DatePicker.SetShowSeconds(false);
                ImGui::OpenPopup(("ReservationTimePicker" + phone).c_str());
            }
            if (m_DatePicker.Render(("ReservationTimePicker" + phone).c_str())) {
                reservationTime = m_DatePicker.GetDateTime();
            }

            // --- Check-in Time with DateTimePicker ---
            ImGui::TableSetColumnIndex(3);

            // Initialize check-in time editor state
            if (m_EditedDateTime.find(phone) == m_EditedDateTime.end()) {
                DateTime init = r->getCheckinTime().GetYear() > 0
                                    ? r->getCheckinTime()
                                    : DateTime::Now();
                m_EditedDateTime[phone] = init;
            }
            DateTime &checkinTime = m_EditedDateTime[phone];

            // DateTimePicker button for check-in time
            std::string checkinButtonText = checkinTime.ToStringDateTime();
            if (ImGui::Button((checkinButtonText + "##checkin" + phone).c_str())) {
                m_DatePicker.SetDateTime(checkinTime);
                m_DatePicker.SetShowTime(true);
                m_DatePicker.SetShowSeconds(false);
                ImGui::OpenPopup(("CheckinTimePicker" + phone).c_str());
            }
            if (m_DatePicker.Render(("CheckinTimePicker" + phone).c_str())) {
                checkinTime = m_DatePicker.GetDateTime();
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
            ImGui::SetNextItemWidth(120);
            if (ImGui::Combo(("##status" + phone).c_str(), &statusIndex,
                             statusLabels.data(), static_cast<int>(statusLabels.size()))) {
                status = static_cast<ReservationStatus>(statusIndex);
            }

            // --- Actions ---
            ImGui::TableSetColumnIndex(5);
            if (ImGui::Button(("Update##" + phone).c_str())) {
                r->setPeopleCount(people);
                r->setTimeOfReservation(reservationTime);
                r->setCheckinTime(checkinTime);
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