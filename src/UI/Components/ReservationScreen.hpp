#pragma once

#include "Screen.hpp"
#include "../../Application/Reservation/ReservationManager.hpp"
#include "../../Infrastructure/Reservation/FileReservationRepository.hpp"
#include "../../Domain/Entity/Reservation/ReservationStatus.hpp"
#include "DateTimePicker.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class ReservationScreen : public Screen {
private:
    std::shared_ptr<ReservationManager> m_Manager;

    // New reservation inputs
    std::vector<char> m_NewPhoneInput;
    std::string m_NewPhoneInputStr;

    // Editing state
    std::unordered_map<std::string, int> m_EditedPeopleCount;
    std::unordered_map<std::string, DateTime> m_EditedDateTime;
    std::unordered_map<std::string, DateTime> m_EditedReservationTime;
    std::unordered_map<std::string, ReservationStatus> m_EditedStatus;

    DateTimePicker m_DatePicker;

public:
    ReservationScreen(Core &core, std::shared_ptr<ReservationManager> manager);

    void Init() override;

    void OnExit() override;

    void Render(float dt) override;

private:
    void DrawNewReservationInput();
    void DrawReservationTable();
    void DrawSaveButton();
    void DrawBackButton();

    const char *StatusToString(ReservationStatus status);
};