#pragma once
#include"IObserver.hpp"
class ISubject{
public:
    virtual bool Attach(IObserver* observer) = 0;
    virtual bool Detach(IObserver* obverser) = 0;
    virtual void notify() = 0;
    virtual ~ISubject() = default;
};
