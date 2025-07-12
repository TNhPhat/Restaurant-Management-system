#include "Table.hpp"

Table::Table() : m_MaxCapacity(4), m_LocationIdentifier(0) {}

Table::Table(int TableID, int MaxCapacity, int LocationIdentifier)
    : m_TableID(TableID), m_MaxCapacity(MaxCapacity), m_LocationIdentifier(LocationIdentifier) {
    SetTableStatus(TableStatus::Free);
}

int Table::GetTableID() const {
    return m_TableID;
}

TableStatus Table::GetTableStatus() const {
    return m_Status;
}

int Table::GetMaxCapacity() const {
    return m_MaxCapacity;
}

int Table::GetLocationIdentifier() const {
    return m_LocationIdentifier;
}

void Table::SetTableID(int TableID)
{
    m_TableID = TableID;
}

void Table::SetTableStatus(const TableStatus &NewTableStatus) {
    m_Status = NewTableStatus;
}

void Table::SetMaxCapacity(int MaxCapacity) {
    m_MaxCapacity = MaxCapacity;
}

void Table::SetLocationIdentifier(int LocationIdentifier) {
    m_LocationIdentifier = LocationIdentifier;
}

bool Table::AddReservation(std::shared_ptr<const Reservation> NewReservation) {
    auto it = std::find_if(m_Reservations.begin(), m_Reservations.end(),
        [&NewReservation](const std::shared_ptr<const Reservation> &r) {
            return r->getPhoneNumber() == NewReservation->getPhoneNumber();
        });

    if (it != m_Reservations.end()) {
        return false; // Already reserved
    }

    m_Reservations.push_back(NewReservation);
    return true;
}

bool Table::RemoveReservation(std::string PhoneNumber) {
    auto originalSize = m_Reservations.size();
    auto it = std::remove_if(m_Reservations.begin(), m_Reservations.end(),
        [PhoneNumber](const std::shared_ptr<const Reservation> &r) {
            return r->getPhoneNumber() == PhoneNumber;
        });
        
    m_Reservations.erase(it, m_Reservations.end());
    return m_Reservations.size() < originalSize;
}

const std::vector<std::shared_ptr<const Reservation>> &Table::GetReservations() const {
    return m_Reservations;
}