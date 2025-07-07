#pragma once
#include "../../Infrastructure/Reservation/FileReservationRepository.hpp"

class ReservationManager
{
private:
    FileReservationRepository &m_ReservationRepository;
public:
    ReservationManager(FileReservationRepository &ReservationRepository);

    ~ReservationManager() = default;

    void AddAReservationByID(int reservationID);
}