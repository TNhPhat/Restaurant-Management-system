#pragma once

#include "IMealRepository.hpp"
#include "Menu/IMenuRepository.hpp"

class FileMealRepository : public IMealRepository {
public:
    FileMealRepository(const std::string &filePath,
                       const IMenuItemRepository &menuItemRepository,
                       const IMenuAddonRepository &menuAddonRepository);

    void SaveMeals(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandler;
};
