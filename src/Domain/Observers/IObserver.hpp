#pragma once
#include<iostream>
class IObserver{
public:
    virtual bool Update() = 0;
    virtual ~IObserver() = 0;
};