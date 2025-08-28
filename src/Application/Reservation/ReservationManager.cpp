#include "ReservationManager.hpp"

ReservationManager::ReservationManager(
    std::unique_ptr<FileReservationRepository> &ReservationRepository): m_ReservationRepository(
    std::move(ReservationRepository)) {
}

void ReservationManager::AddAReservationByPhoneNumber(std::string PhoneNumber) {
    if (m_ReservationRepository->GetReservationByPhoneNumber(PhoneNumber) != nullptr) {
        LOG_WARNING("Reservation with phone number {} already exists. Skipping.", PhoneNumber);
        return;
    }

    auto newReservation = std::make_shared<Reservation>();
    newReservation->setPhoneNumber(PhoneNumber);

    // Set current time as reservation time
    newReservation->setTimeOfReservation(DateTime::Now());
    newReservation->setPeopleCount(1);
    newReservation->setReservationStatus(ReservationStatus::Pending);
    newReservation->setCheckinTime(DateTime());

    m_ReservationRepository->SaveReservation(newReservation);

    LOG_INFO("New reservation with phone number {} added.", PhoneNumber);
}

void ReservationManager::RemoveReservationByPhoneNumber(std::string PhoneNumber) {
    m_ReservationRepository->RemoveReservation(PhoneNumber);
}

std::vector<std::shared_ptr<Reservation> > ReservationManager::GetAllReservations() const {
    return m_ReservationRepository->GetReservations();
}

void ReservationManager::SaveReservations(std::string filePath) const {
    m_ReservationRepository->SaveReservations(filePath);
}

void ReservationManager::RemoveReservation(std::string phone) {
    m_ReservationRepository->RemoveReservation(phone);
}

std::vector<std::shared_ptr<Reservation> >
ReservationManager::GetReservationsInRange(DateTime from, DateTime to) const {
    std::vector<std::shared_ptr<Reservation> > result;
    auto allReservations = m_ReservationRepository->GetReservations();
    std::copy_if(allReservations.begin(), allReservations.end(), std::back_inserter(result),
                 [&from, &to](const std::shared_ptr<Reservation> &res) {
                     DateTime resTime = res->getTimeOfReservation();
                     return resTime >= from && resTime <= to;
                 });
    return result;
}
