#include"UsingResourceSubject.hpp"
bool UsingResourceSubject::Attach(IObserver* observer) {
    m_Observers.push_back(observer);
}

bool UsingResourceSubject::Detach(IObserver* observer) {
    m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void UsingResourceSubject::notify() {
    for (auto* obs : m_Observers) {
        obs->Update();
    }
}