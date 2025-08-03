#pragma once
#include "../../Infrastructure/Reservation/FileReservationRepository.hpp"
#include <string>
#include <memory>
#include <vector>

class ReservationManager
{
private:
    FileReservationRepository &m_ReservationRepository;
public:
    ReservationManager(FileReservationRepository &ReservationRepository);

    ~ReservationManager() = default;

    void AddAReservationByPhoneNumber(std::string PhoneNumber);
    void RemoveReservationByPhoneNumber(std::string PhoneNumber);
    std::vector<std::shared_ptr<Reservation>> GetAllReservations() const;
};