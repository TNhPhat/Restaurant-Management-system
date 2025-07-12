#include "ReservationManager.hpp"

ReservationManager::ReservationManager(FileReservationRepository &ReservationRepository): 
    m_ReservationRepository(ReservationRepository) {}

void ReservationManager::AddAReservationByPhoneNumber(std::string PhoneNumber)
{
    if (m_ReservationRepository.GetReservationByPhoneNumber(PhoneNumber) != nullptr)
    {
        LOG_WARNING("Reservation with phone number {} already exists. Skipping.", PhoneNumber);
        return;
    }

    auto newReservation = std::make_shared<Reservation>();
    newReservation->setPhoneNumber(PhoneNumber);

    // newReservation->setTimeOfReservation(DateTime::Now());
    newReservation->setPeopleCount(1);
    newReservation->setReservationStatus(ReservationStatus::Pending);
    newReservation->setCheckinTime(DateTime());

    m_ReservationRepository.SaveReservation(newReservation);

    LOG_INFO("New reservation with phone number {} added.", PhoneNumber);
}