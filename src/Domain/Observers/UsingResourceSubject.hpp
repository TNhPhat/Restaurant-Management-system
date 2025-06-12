#pragma once
#include<vector>
#include"ISubject.hpp"
class UsingResourceSubject : public ISubject {
public:
    bool Attach(IObserver* observer) override;
    bool Detach(IObserver* observer) override;
    void notify() override;

private:
    std::vector<IObserver*> m_Observers;
};