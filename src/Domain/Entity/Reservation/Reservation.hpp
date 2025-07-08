#pragma once
#include "FileHandle.hpp"
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
    Reservation();
    Reservation(DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime);
    void setID(int id);
    void setTimeOfReservation(DateTime TimeOfReservation);
    void setPeopleCount(int count);
    void setReservationStatus(ReservationStatus Status);
    void setCheckinTime(DateTime CheckinTime);
    int getID() const;
    DateTime getTimeOfReservation() const;
    int getPeopleCount() const;
    ReservationStatus getStatus() const;
    DateTime getCheckinTime() const;
};