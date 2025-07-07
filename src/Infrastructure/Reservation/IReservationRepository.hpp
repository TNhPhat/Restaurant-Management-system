#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Reservation/Reservation.hpp"

class IReservationRepository {
public:
    ~IReservationRepository() = default;

    virtual void SaveReservations(const std::string &filePath) const = 0;

    std::shared_ptr<Reservation> SaveReservation(const std::shared_ptr<Reservation> &reservation);

    void RemoveReservation(const std::shared_ptr<Reservation> &reservation);

    void RemoveReservation(int reservationID);

    std::vector<std::shared_ptr<Reservation>> GetReservations() const;
    std::vector<std::shared_ptr<Reservation>> GetReservationsSortedByTimeOfReservation() const;
    std::shared_ptr<Reservation> GetReservationByID(int reservationID) const;

private:
    std::vector<std::shared_ptr<Reservation> > m_Reservations;
};