#include "ITableRepository.hpp"

std::shared_ptr<Table> ITableRepository::SaveTable(const std::shared_ptr<Table> &table)
{
    if (table == nullptr)
    {
        LOG_ERROR("Error saving Table: Table is null.");
        return nullptr;
    }

    const auto existingTable = this->GetTableByID(table->GetTableID());
    if (existingTable == nullptr)
    {
        this->m_Tables.push_back(table);
        LOG_INFO("Table with ID {} saved successfully.", table->GetTableID());
        return table;
    }

    existingTable->SetMaxCapacity(table->GetMaxCapacity());
    existingTable->SetLocationIdentifier(table->GetLocationIdentifier());
    existingTable->SetTableStatus(table->GetTableStatus());

    LOG_INFO("Table with ID {} updated successfully.", table->GetTableID());
    return existingTable;
}

void ITableRepository::RemoveTable(const std::shared_ptr<Table> &table)
{
    if (table == nullptr)
    {
        LOG_ERROR("Error removing Table: Table is null.");
        return;
    }

    this->RemoveTable(table->GetTableID());
    LOG_INFO("Table with ID {} removed successfully.", table->GetTableID());
}

void ITableRepository::RemoveTable(int tableID)
{
    for (const auto &table : this->m_Tables)
    {
        if (table->GetTableID() == tableID)
        {
            LOG_INFO("Removing Table with ID {}.", tableID);
            std::erase(this->m_Tables, table);
            return;
        }
    }
    LOG_ERROR("Table with ID {} not found.", tableID);
}

std::vector<std::shared_ptr<Table>> ITableRepository::GetTables() const
{
    return this->m_Tables;
}

std::vector<std::shared_ptr<Table>> ITableRepository::GetTablesByMaxCapacity(int capacity) const
{
    std::vector<std::shared_ptr<Table>> sorted;

    for (const auto& table : m_Tables)
    {
        if (table && table->GetMaxCapacity() <= capacity)
        {
            sorted.push_back(table);
        }
    }

    return sorted;
}

std::shared_ptr<Table> ITableRepository::GetTableByID(int tableID) const
{
    for (const auto &table : this->m_Tables)
    {
        if (table->GetTableID() == tableID)
        {
            return table;
        }
    }
    return nullptr;
}