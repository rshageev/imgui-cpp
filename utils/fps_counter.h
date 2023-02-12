#pragma once

#include <array>

class ImGuiFPSCounter
{
public:
    ImGuiFPSCounter() { FramerateSecPerFrame.fill(0.0f); }

    float UpdateCurrentFPS(float dt);

private:
    std::array<float, 60> FramerateSecPerFrame;
    size_t FramerateSecPerFrameIdx = 0;
    size_t FramerateSecPerFrameCount = 0;
    float FramerateSecPerFrameAccum = 0.0f;
};
