#include "TableManager.hpp"

TableManager::TableManager(FileTableRepository &TableRepository): 
    m_TableRepository(TableRepository) {}

void TableManager::AddATableByID(int tableID)
{
    if (m_TableRepository.GetTableByID(tableID) != nullptr)
    {
        LOG_WARNING("Table with ID {} already exists. Skipping.", tableID);
        return;
    }

    auto newTable = std::make_shared<Table>();
    newTable->SetTableID(tableID);

    newTable->SetMaxCapacity(4);  // Default to 4 people
    newTable->SetTableStatus(TableStatus::Free);  // Set status as available

    m_TableRepository.SaveTable(newTable);

    LOG_INFO("New table with ID {} added successfully.", tableID);
}