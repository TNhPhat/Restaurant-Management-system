#pragma once

#include "IMealRepository.hpp"
#include "Menu/IMenuRepository.hpp"

class FileMealRepository : public IMealRepository {
public:
    FileMealRepository(const std::string &filePath,
                       const std::shared_ptr<IMenuItemRepository> &menuItemRepository,
                       const std::shared_ptr<IMenuAddonRepository> &menuAddonRepository);

    void SaveMeals(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandler;
};
