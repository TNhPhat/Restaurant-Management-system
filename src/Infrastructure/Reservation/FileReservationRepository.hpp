#pragma once

#include "IReservationRepository.hpp"

class FileReservationRepository : public IReservationRepository {
public:
    FileReservationRepository(std::string ilePath);

    void SaveReservations(std::string filePath) const override;

private:
    std::unique_ptr<JsonHandle> m_FileHandle;
};
