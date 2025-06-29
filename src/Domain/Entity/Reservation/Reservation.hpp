#pragma once

#include"ReservationStatus.hpp"
#include "../../ValueObject/DateTime.hpp"

class Reservation
{
private:
    int m_ReservationID;
    DateTime m_TimeOfReservation;
    int m_PeopleCount;
    ReservationStatus m_Status;
    DateTime m_CheckinTime;
public:
    Reservation(DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime);
    void setPeopleCount(int count);
    int getID() const;
    DateTime getTimeOfReservation() const;
    int getPeopleCount() const;
    ReservationStatus getStatus() const;
    DateTime getCheckinTime() const;
};