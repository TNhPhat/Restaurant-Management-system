#pragma once
#include "FileHandle.hpp"
#include"ReservationStatus.hpp"
#include "../../ValueObject/DateTime.hpp"
#include<string>
class Reservation
{
private:
    std::string m_PhoneNumber;
    DateTime m_TimeOfReservation;
    int m_PeopleCount;
    ReservationStatus m_Status;
    DateTime m_CheckinTime;
public:
    Reservation();
    Reservation(std::string PhoneNumber, DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime);
    void setPhoneNumber(std::string PhoneNumber);
    void setTimeOfReservation(DateTime TimeOfReservation);
    void setPeopleCount(int count);
    void setReservationStatus(ReservationStatus Status);
    void setCheckinTime(DateTime CheckinTime);
    std::string getPhoneNumber() const;
    DateTime getTimeOfReservation() const;
    int getPeopleCount() const;
    ReservationStatus getStatus() const;
    DateTime getCheckinTime() const;
};