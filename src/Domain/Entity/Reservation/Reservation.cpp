#include "Reservation.hpp"

Reservation::Reservation(DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime)
{
    this->m_TimeOfReservation = TimeOfReservation;
    this->m_PeopleCount = PeopleCount;
    this->m_Status = ReservationStatus::Requested;
    this->m_CheckinTime = CheckinTime;
}

// Setters
void Reservation::setID(int id)
{
    this->m_ReservationID = id;
}

void Reservation::setTimeOfReservation(DateTime TimeOfReservation)
{
    this->m_TimeOfReservation = TimeOfReservation;
}

void Reservation::setPeopleCount(int count)
{
    this->m_PeopleCount = count;
}

void Reservation::setReservationStatus(ReservationStatus Status)
{
    this->m_Status = Status;
}

void Reservation::setCheckinTime(DateTime CheckinTime)
{
    this->m_CheckinTime = CheckinTime;
}

// Getters
int Reservation::getID() const
{
    return this->m_ReservationID;
}

DateTime Reservation::getTimeOfReservation() const
{
    return this->m_TimeOfReservation;
}

int Reservation::getPeopleCount() const
{
    return this->m_PeopleCount;
}

ReservationStatus Reservation::getStatus() const
{
    return this->m_Status;
}

DateTime Reservation::getCheckinTime() const
{
    return this->m_CheckinTime;
}
