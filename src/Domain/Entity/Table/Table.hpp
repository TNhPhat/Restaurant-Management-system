#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include "TableStatus.hpp"
#include "Reservation.hpp"

class Table
{
private:
    const int m_TableID;
    TableStatus m_Status;
    int m_MaxCapacity;
    int m_LocationIdentifier;
    std::vector<std::shared_ptr<const Reservation>> m_Reservations;
public:
    Table(int TableID, int MaxCapacity, int LocationIdentifier);
    ~Table() = default;
    int GetMaxCapacity() const;
    int GetLocationIdentifier() const;

    void SetTableStatus(const TableStatus &NewTableStatus);

    bool AddReservation(const std::shared_ptr<const Reservation> NewReservation);
    bool RemoveReservation(int ReservationID);
    const std::vector<std::shared_ptr<const Reservation>> &GetReservations() const;
};