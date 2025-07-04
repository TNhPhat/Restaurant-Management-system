#pragma once

#include "IMenuRepository.hpp"

class FileMenuAddonRepository : public IMenuAddonRepository {
public:
    FileMenuAddonRepository(std::string filePath);

    void SaveMenuAddons(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandle;
};

class FileMenuItemRepository : public IMenuItemRepository {
public:
    FileMenuItemRepository(std::string filePath, const std::shared_ptr<IMenuAddonRepository> &menuAddonRepository);

    void SaveMenuItems(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandle;
};

class FileMenuRepository : public IMenuRepository {
public:
    FileMenuRepository(std::string filePath, const std::shared_ptr<IMenuItemRepository> &menuItemRepository);

    void SaveMenu(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandle;
};
