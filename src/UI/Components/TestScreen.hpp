#pragma once
#include "Screen.hpp"

class TestScreenA: public Screen {
public:
    TestScreenA(Core& core);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;
};

class TestScreenB: public Screen {
public:
    TestScreenB(Core& core);

    void Render(float DT) override;

    void Init() override;

    void OnExit() override;
};