#include "ReservationScreen.hpp"
#include "imgui.h"
#include "UI/UICore/Core.hpp"
#include <array>

ReservationScreen::ReservationScreen(Core& core, ReservationManager& manager, FileReservationRepository& repo)
    : Screen(core), m_Manager(manager), m_FileRepo(repo) {
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

    DrawNewReservationInput();
    ImGui::Spacing();

    DrawReservationTable();
    ImGui::Spacing();

    DrawSaveButton();
    ImGui::SameLine();
    DrawBackButton();
}

// Draw input + button to add new reservation
void ReservationScreen::DrawNewReservationInput() {
    if (ImGui::InputText("New Phone Number", m_NewPhoneInput.data(), m_NewPhoneInput.size())) {
        // InputText modifies the buffer, so we need to update our string
        m_NewPhoneInputStr = std::string(m_NewPhoneInput.data());
    }
    
    if (ImGui::Button("Add")) {
        if (!m_NewPhoneInputStr.empty()) {
            m_Manager.AddAReservationByPhoneNumber(m_NewPhoneInputStr);
            m_NewPhoneInputStr.clear();
            std::fill(m_NewPhoneInput.begin(), m_NewPhoneInput.end(), '\0');
        }
    }
}

// Draw table showing all reservations and allow edit/delete
void ReservationScreen::DrawReservationTable() {
    // Use GetReservations() instead of GetAllReservations()
    const auto reservations = m_FileRepo.GetReservations();

    if (ImGui::BeginTable("Reservations", 6, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg)) {
        ImGui::TableSetupColumn("Phone");
        ImGui::TableSetupColumn("People");
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Check-in");
        ImGui::TableSetupColumn("Status");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        for (const auto& r : reservations) {
            const std::string& phone = r->getPhoneNumber();
            ImGui::TableNextRow();

            // Phone (non-editable)
            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", phone.c_str());

            // PeopleCount
            ImGui::TableSetColumnIndex(1);
            int& people = m_EditedPeopleCount[phone];
            if (people == 0) people = r->getPeopleCount(); // default
            ImGui::InputInt(("##people" + phone).c_str(), &people);

            // TimeOfReservation (read-only)
            ImGui::TableSetColumnIndex(2);
            ImGui::Text("%s", r->getTimeOfReservation().ToStringDateTime().c_str());

            // Check-in time (editable string)
            ImGui::TableSetColumnIndex(3);
            
            // Initialize buffer if it doesn't exist
            if (m_EditedCheckinBuffers.find(phone) == m_EditedCheckinBuffers.end()) {
                m_EditedCheckinBuffers[phone].resize(256);
                std::string checkinStr = r->getCheckinTime().ToStringDateTime();
                std::strncpy(m_EditedCheckinBuffers[phone].data(), checkinStr.c_str(), 
                           std::min(checkinStr.length(), m_EditedCheckinBuffers[phone].size() - 1));
            }
            
            ImGui::InputText(("##checkin" + phone).c_str(), 
                           m_EditedCheckinBuffers[phone].data(), 
                           m_EditedCheckinBuffers[phone].size());

            // Status combo
            ImGui::TableSetColumnIndex(4);
            ReservationStatus& status = m_EditedStatus[phone];
            if (status == ReservationStatus::Requested && r->getStatus() != ReservationStatus::Requested)
                status = r->getStatus(); // preserve

            static const std::array<const char*, 3> statusLabels = {
                "Requested", "Pending", "CheckedIn"
            };

            int statusIndex = static_cast<int>(status);
            if (ImGui::Combo(("##status" + phone).c_str(), &statusIndex, statusLabels.data(), static_cast<int>(statusLabels.size()))) {
                status = static_cast<ReservationStatus>(statusIndex);
            }

            // Actions
            ImGui::TableSetColumnIndex(5);
            if (ImGui::Button(("Update##" + phone).c_str())) {
                r->setPeopleCount(people);
                std::string checkinStr(m_EditedCheckinBuffers[phone].data());
                r->setCheckinTime(DateTime::FromDateTimeString(checkinStr));
                r->setReservationStatus(status);
            }
            ImGui::SameLine();
            if (ImGui::Button(("Delete##" + phone).c_str())) {
                // Use the repository's RemoveReservation method
                m_FileRepo.RemoveReservation(phone);
            }
        }
        ImGui::EndTable();
    }
}

// Save to file
void ReservationScreen::DrawSaveButton() {
    if (ImGui::Button("Save All")) {
        m_FileRepo.SaveReservations("Reservation.json");
    }
}

// Back to previous screen
void ReservationScreen::DrawBackButton() {
    if (ImGui::Button("Back")) {
        m_Core.PopScreen();
    }
}

const char* ReservationScreen::StatusToString(ReservationStatus status) {
    switch (status) {
        case ReservationStatus::Requested: return "Requested";
        case ReservationStatus::Pending: return "Pending";
        case ReservationStatus::CheckedIn: return "CheckedIn";
        default: return "Unknown";
    }
}