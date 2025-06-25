#pragma once

#include"ReservationStatus.hpp"
#include "../../ValueObject/DateTime.hpp"
//#include "./Customer/Customer.hpp"

class Reservation
{
private:
    int m_ReservationID;
    DateTime m_TimeOfReservation;
    int m_PeopleCount;
    ReservationStatus m_Status;
    DateTime m_CheckinTime;
    //Customer m_Customer
public:
    Reservation(DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime);
    void setPeopleCount(int count);
    int getID() const;
    DateTime getTimeOfReservation() const;
    int getPeopleCount() const;
    ReservationStatus getStatus() const;
    DateTime getCheckinTime() const;
};