#include "ReservationManager.hpp"

ReservationManager::ReservationManager(FileReservationRepository &ReservationRepository): 
    m_ReservationRepository(ReservationRepository) {}

void ReservationManager::AddAReservationByID(int reservationID)
{
    if (m_ReservationRepository.GetReservationByID(reservationID) != nullptr)
    {
        LOG_WARN("Reservation with ID {} already exists. Skipping.", reservationID);
        return;
    }

    auto newReservation = std::make_shared<Reservation>();
    newReservation->setID(reservationID);

    // newReservation->setTimeOfReservation(DateTime::Now());
    newReservation->setPeopleCount(1);
    newReservation->setReservationStatus(ReservationStatus::Pending);
    newReservation->setCheckinTime(DateTime());

    m_ReservationRepository.SaveReservation(newReservation);

    LOG_INFO("New reservation with ID {} added.", reservationID);
}