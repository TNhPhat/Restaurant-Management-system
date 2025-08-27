#pragma once
#include <memory>

#include "Screen.hpp"
#include "Application/Reservation/ReservationManager.hpp"

class Dashboard : public Screen {
    std::shared_ptr<ReservationManager> m_ReservationManager;

public:
    Dashboard(Core &core);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;
};
