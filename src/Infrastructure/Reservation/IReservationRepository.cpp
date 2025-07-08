#include "IReservationRepository.hpp"

std::shared_ptr<Reservation> IReservationRepository::SaveReservation(std::shared_ptr<Reservation> reservation)
{
    if (reservation == nullptr)
    {
        LOG_ERROR("Error saving Reservation: Reservation is null.");
        return nullptr;
    }

    const auto existing = this->GetReservationByID(reservation->getID());
    if (existing == nullptr)
    {
        this->m_Reservations.push_back(reservation);
        LOG_INFO("Reservation with ID {} saved successfully.", reservation->getID());
        return reservation;
    }

    existing->setTimeOfReservation(reservation->getTimeOfReservation());
    existing->setPeopleCount(reservation->getPeopleCount());
    existing->setReservationStatus(reservation->getStatus());
    existing->setCheckinTime(reservation->getCheckinTime());

    LOG_INFO("Reservation with ID {} updated successfully.", reservation->getID());
    return existing;
}

void IReservationRepository::RemoveReservation(const std::shared_ptr<Reservation> &reservation)
{
    if (reservation == nullptr)
    {
        LOG_ERROR("Error removing Reservation: Reservation is null.");
        return;
    }

    this->RemoveReservation(reservation->getID());
    LOG_INFO("Reservation with ID {} removed successfully.", reservation->getID());
}

void IReservationRepository::RemoveReservation(int reservationID)
{
    for (const auto &r : this->m_Reservations)
    {
        if (r->getID() == reservationID)
        {
            LOG_INFO("Removing Reservation with ID {}.", reservationID);
            std::erase(this->m_Reservations, r);
            return;
        }
    }
    LOG_ERROR("Reservation with ID {} not found.", reservationID);
}

std::vector<std::shared_ptr<Reservation>> IReservationRepository::GetReservations() const
{
    return this->m_Reservations;
}

std::vector<std::shared_ptr<Reservation>> IReservationRepository::GetReservationsByTimeOfReservation() const
{
    std::vector<std::shared_ptr<Reservation>> sorted = this->m_Reservations;

    std::sort(sorted.begin(), sorted.end(), [](const std::shared_ptr<Reservation>& a, const std::shared_ptr<Reservation>& b) {
        const DateTime& t1 = a->getTimeOfReservation();
        const DateTime& t2 = b->getTimeOfReservation();

        // Compare year, month, day, hour, minute, second
        if (t1.GetYear() != t2.GetYear()) return t1.GetYear() < t2.GetYear();
        if (t1.GetMonth() != t2.GetMonth()) return t1.GetMonth() < t2.GetMonth();
        if (t1.GetDay() != t2.GetDay()) return t1.GetDay() < t2.GetDay();
        if (t1.GetHour() != t2.GetHour()) return t1.GetHour() < t2.GetHour();
        if (t1.GetMinute() != t2.GetMinute()) return t1.GetMinute() < t2.GetMinute();
        return t1.GetSecond() < t2.GetSecond();
    });

    return sorted;
}

std::shared_ptr<Reservation> IReservationRepository::GetReservationByID(int reservationID) const
{
    for (const auto &r : this->m_Reservations)
    {
        if (r->getID() == reservationID)
        {
            return r;
        }
    }
    return nullptr;
}