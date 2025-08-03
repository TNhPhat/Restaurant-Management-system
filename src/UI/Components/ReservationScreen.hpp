#pragma once

#include "Screen.hpp"
#include "../../Application/Reservation/ReservationManager.hpp"
#include "../../Infrastructure/Reservation/FileReservationRepository.hpp"
#include "../../Domain/Entity/Reservation/ReservationStatus.hpp"

#include <string>
#include <unordered_map>
#include <vector>

class ReservationScreen : public Screen {
private:
    ReservationManager& m_Manager;
    FileReservationRepository& m_FileRepo;
    
    std::vector<char> m_NewPhoneInput;
    std::string m_NewPhoneInputStr;
    
    std::unordered_map<std::string, int> m_EditedPeopleCount;
    std::unordered_map<std::string, std::vector<char>> m_EditedCheckinBuffers;
    std::unordered_map<std::string, ReservationStatus> m_EditedStatus;

public:
    ReservationScreen(Core& core, ReservationManager& manager, FileReservationRepository& repo);
    
    void Init() override;
    void OnExit() override;
    void Render(float dt) override;

private:
    void DrawNewReservationInput();
    void DrawReservationTable();
    void DrawSaveButton();
    void DrawBackButton();
    
    const char* StatusToString(ReservationStatus status);
};