#pragma once
#include "../../Infrastructure/Reservation/FileReservationRepository.hpp"
#include<string>

class ReservationManager
{
private:
    FileReservationRepository &m_ReservationRepository;
public:
    ReservationManager(FileReservationRepository &ReservationRepository);

    ~ReservationManager() = default;

    void AddAReservationByPhoneNumber(std::string PhoneNumber);
};