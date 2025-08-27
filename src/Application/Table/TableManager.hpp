#pragma once
#include "../../Infrastructure/Table/FileTableRepository.hpp"

class TableManager
{
private:
    std::shared_ptr<FileTableRepository> m_TableRepository;
public:
    TableManager(std::shared_ptr<FileTableRepository> &TableRepository);

    ~TableManager() = default;
    
    void AddATableByID(int tableID);
};