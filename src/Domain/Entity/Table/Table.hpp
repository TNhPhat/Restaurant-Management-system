#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <string>
#include "FileHandle.hpp"
#include "TableStatus.hpp"
#include "Reservation.hpp"

class Table
{
private:
    int m_TableID;
    TableStatus m_Status;
    int m_MaxCapacity;
    int m_LocationIdentifier;
    std::vector<std::shared_ptr<const Reservation>> m_Reservations;
public:
    Table();
    Table(int TableID, int MaxCapacity, int LocationIdentifier);
    ~Table() = default;
    int GetTableID() const;
    TableStatus GetTableStatus() const; 
    int GetMaxCapacity() const;
    int GetLocationIdentifier() const;

    void SetTableID(int TableID);
    void SetTableStatus(const TableStatus &NewTableStatus);
    void SetMaxCapacity(int MaxCapacity);
    void SetLocationIdentifier(int LocationIdentifier);

    bool AddReservation(const std::shared_ptr<const Reservation> NewReservation);
    bool RemoveReservation(std::string PhoneNumber);
    const std::vector<std::shared_ptr<const Reservation>> &GetReservations() const;
};