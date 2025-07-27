#pragma once
#include "imgui.h"

namespace Constants {
    //Customer
    constexpr float POINTS_PER_VND = 1.0f / 10000.0f;
    constexpr int VND_PER_POINT = 1000;

    constexpr int WINDOW_WIDTH = 1280;
    constexpr int WINDOW_HEIGHT = 720;

    constexpr std::string GLSL_VERSION = "#version 460";

    constexpr int SLEEPTIME = 10;

    constexpr auto CLEAR_COLOR = ImColor(32, 32, 32, 255);

    //
};
