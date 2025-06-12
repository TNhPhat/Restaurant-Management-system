#pragma once
#include "Reservation.hpp"
#include "../ValueObject/DateTime.hpp"
//#include "./Customer/Customer.hpp"

class Reservation
{
private:
    const int m_ReservationID;
    DateTime m_TimeOfReservation;
    int m_PeopleCount;
    ReservationStatus m_Status;
    DateTime m_CheckinTime;
    //Customer m_Customer
public:
    Reservation(DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime);
    void setPeopleCount();
    int getID();
    DateTime getTimeOfReservation();
    int getPeopeCount();
    ReservationStatus getStatus();
    DateTime getCheckinTime;
}