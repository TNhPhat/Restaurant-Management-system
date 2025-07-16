#include "Reservation.hpp"

Reservation::Reservation() : m_PhoneNumber("0"), m_TimeOfReservation(DateTime::Now()), m_PeopleCount(0), m_CheckinTime(DateTime::Now()) {}

Reservation::Reservation(std::string PhoneNumber, DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime)
{
    this->m_PhoneNumber = PhoneNumber;
    this->m_TimeOfReservation = TimeOfReservation;
    this->m_PeopleCount = PeopleCount;
    this->m_Status = ReservationStatus::Requested;
    this->m_CheckinTime = CheckinTime;
}

// Setters
void Reservation::setPhoneNumber(std::string PhoneNumber)
{
    this->m_PhoneNumber = PhoneNumber;
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
std::string Reservation::getPhoneNumber() const
{
    return this->m_PhoneNumber;
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
