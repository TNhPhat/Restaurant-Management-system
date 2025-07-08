#include "FileTableRepository.hpp"

FileTableRepository::FileTableRepository(std::string filePath) {
    this->m_FileHandler = std::make_unique<JsonHandle>();
    this->m_FileHandler->LoadFile(filePath);
    auto data = this->m_FileHandler->GetDaTa();

    if (!data.is_array()) {
        LOG_ERROR("FileTableRepository: Invalid data format in file {}", filePath);
        return;
    }

    for (const auto& tableData : data) {
        int id = tableData["ID"].get<int>();
        int maxCapacity = tableData["MaxCapacity"].get<int>();
        int location = tableData["LocationIdentifier"].get<int>();
        TableStatus status = static_cast<TableStatus>(tableData["Status"].get<int>());

        auto table = std::make_shared<Table>(id, maxCapacity, location);
        table->SetTableStatus(status);

        this->ITableRepository::SaveTable(table);
    }
}

void FileTableRepository::SaveTables(std::string filePath) const {
    json data = json::array();

    for (const auto& t : this->GetTables()) {
        json tJson;
        tJson["ID"] = t->GetTableID();
        tJson["MaxCapacity"] = t->GetMaxCapacity();
        tJson["LocationIdentifier"] = t->GetLocationIdentifier();
        tJson["Status"] = static_cast<int>(t->GetTableStatus());

        data.push_back(tJson);
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }

    outFile << data.dump(4);
    outFile.close();
    LOG_INFO("Tables saved successfully to {}", filePath);
}