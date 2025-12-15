#pragma once

#include <Window.hpp>
#include <array>

namespace Pong
{
    struct Resolution
    {
        int Width{}, Height{};
    };
    constexpr size_t VALID_RESOLUTION_COUNT = 4;
    std::array<Resolution, 4> ValidResolutions
    {{
        {1280, 720},
        {1920, 1080},
        {2560, 1440},
        {3840, 2160}
    }};
}