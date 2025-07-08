#pragma once

#include "ITableRepository.hpp"

class FileTableRepository : public ITableRepository {
public:
    FileTableRepository(std::string filePath);

    void SaveTables(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandler;
};