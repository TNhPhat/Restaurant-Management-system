#include "TableManager.hpp"

TableManager::TableManager(std::shared_ptr<FileTableRepository> &TableRepository): m_TableRepository(TableRepository) {
}

std::vector<std::shared_ptr<Table> > TableManager::GetTables() {
    return m_TableRepository->GetTables();
}

std::vector<std::shared_ptr<Table> > TableManager::GetFreeTables() const {
    std::vector<std::shared_ptr<Table> > freeTables;
    auto allTables = m_TableRepository->GetTables();
    std::copy_if(allTables.begin(), allTables.end(), std::back_inserter(freeTables),
                 [](const std::shared_ptr<Table> &table) {
                     return table->GetTableStatus() == TableStatus::Free;
                 });
    return freeTables;
}

void TableManager::AddATableByID(int tableID) {
    if (m_TableRepository->GetTableByID(tableID) != nullptr) {
        LOG_WARNING("Table with ID {} already exists. Skipping.", tableID);
        return;
    }

    auto newTable = std::make_shared<Table>();
    newTable->SetTableID(tableID);

    newTable->SetMaxCapacity(4); // Default to 4 people
    newTable->SetTableStatus(TableStatus::Free); // Set status as available

    m_TableRepository->SaveTable(newTable);

    LOG_INFO("New table with ID {} added successfully.", tableID);
}
