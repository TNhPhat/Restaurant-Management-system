#pragma once
#include "../../Infrastructure/Table/FileTableRepository.hpp"

class TableManager
{
private:
    FileTableRepository &m_TableRepository;
public:
    TableManager(FileTableRepository &TableRepository);

    ~TableManager() = default;
    
    void AddATableByID(int tableID);
};