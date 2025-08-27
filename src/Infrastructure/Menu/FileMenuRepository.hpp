#pragma once

#include "IMenuRepository.hpp"

class FileMenuRepository : public IMenuRepository {
public:
    FileMenuRepository(std::string MenuFilePath, std::string ItemFilePath, std::string AddonFilePath);

    void SaveMenus(std::string filePath) const override;

    void SaveItems(std::string filePath) const override;

    void SaveAddons(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandle;
};
