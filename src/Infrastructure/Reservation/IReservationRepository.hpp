#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Reservation/Reservation.hpp"

class IReservationRepository {
public:
    virtual ~IReservationRepository() = default;

    virtual void SaveReservations(std::string filePath) const = 0;

    std::shared_ptr<Reservation> SaveReservation(std::shared_ptr<Reservation> reservation);

    void RemoveReservation(const std::shared_ptr<Reservation> &reservation);

    void RemoveReservation(std::string PhoneNumber);

    std::vector<std::shared_ptr<Reservation>> GetReservations() const;
    std::vector<std::shared_ptr<Reservation>> GetReservationsByTimeOfReservation() const;
    std::shared_ptr<Reservation> GetReservationByPhoneNumber(std::string PhoneNumber) const;

private:
    std::vector<std::shared_ptr<Reservation> > m_Reservations;
};