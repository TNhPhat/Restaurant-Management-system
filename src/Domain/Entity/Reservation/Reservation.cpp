Reservation::Reservation(DateTime TimeOfReservation, int PeopleCount, DateTime CheckinTime)
{
    this->m_TimeOfReservation = TimeOfReservation;
    this->m_PeopleCount = PeopleCount;
    this->m_Status = ReservationStatus::Requested; // Not sure
    this->m_CheckinTime = CheckinTime;
}

void Reservation::setPeopleCount(int count)
{
    this->m_PeopleCount = count;
}

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