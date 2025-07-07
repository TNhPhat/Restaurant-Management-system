#pragma once

#include <memory>
#include <vector>
#include <string>
#include "Table/Table.hpp"

class ITableRepository {
public:
    virtual ~ITableRepository() = default;

    virtual void SaveTables(const std::string &filePath) const = 0;

    std::shared_ptr<Table> SaveTable(const std::shared_ptr<Table> &table);

    void RemoveTable(const std::shared_ptr<Table> &table);

    void RemoveTable(int tableID);

    std::vector<std::shared_ptr<Table>> GetTables() const;
    std::vector<std::shared_ptr<Table>> GetTablesByMaxCapacity(int capacity) const;
    std::shared_ptr<Table> GetTableByID(int tableID) const;

private:
    std::vector<std::shared_ptr<Table>> m_Tables;
};