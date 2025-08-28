#include "FileReservationRepository.hpp"

FileReservationRepository::FileReservationRepository(std::string filePath) {
    this->m_FileHandle = std::make_unique<JsonHandle>();
    this->m_FileHandle->LoadFile(filePath);
    auto data = this->m_FileHandle->GetDaTa();

    if (!data.is_array()) {
        LOG_ERROR("FileReservationRepository: Invalid data format in file {}", filePath);
        return;
    }

    for (const auto &resData: data) {
        std::string PhoneNumber = resData["PhoneNumber"].get<std::string>();
        DateTime timeOfReservation = DateTime::FromDateTimeString(resData["TimeOfReservation"].get<std::string>());
        int peopleCount = resData["PeopleCount"].get<int>();
        ReservationStatus status = static_cast<ReservationStatus>(resData["Status"].get<int>());
        DateTime checkinTime = DateTime::FromDateTimeString(resData["CheckinTime"].get<std::string>());

        auto reservation = std::make_shared<Reservation>(PhoneNumber, timeOfReservation, peopleCount, checkinTime);
        reservation->setPhoneNumber(PhoneNumber);
        reservation->setReservationStatus(status);

        this->SaveReservation(reservation);
    }
}

void FileReservationRepository::SaveReservations(std::string filePath) const {
    json data = json::array();

    for (const auto &r: this->GetReservations()) {
        json rJson;
        rJson["PhoneNumber"] = r->getPhoneNumber();
        rJson["TimeOfReservation"] = r->getTimeOfReservation().ToStringDateTime();
        rJson["PeopleCount"] = r->getPeopleCount();
        rJson["Status"] = static_cast<int>(r->getStatus());
        rJson["CheckinTime"] = r->getCheckinTime().ToStringDateTime();

        data.push_back(rJson);
    }

    std::ofstream outFile(filePath);
    if (!outFile.is_open()) {
        LOG_ERROR("Failed to open file {} for writing.", filePath);
        return;
    }

    outFile << data.dump(4);
    outFile.close();
    LOG_INFO("Reservations saved successfully to {}", filePath);
}
