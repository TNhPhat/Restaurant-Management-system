#pragma once

#include "IRestaurantRepository.hpp"
#include "Menu/IMenuRepository.hpp"

class FileRestaurantRepository : public IRestaurantRepository
{
public:
    FileRestaurantRepository(std::string filePath, const IMenuRepository &menuRepository);

    void SaveRestaurantData(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandle;
};
