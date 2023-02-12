#include "fps_counter.h"

float ImGuiFPSCounter::UpdateCurrentFPS(float dt)
{
    FramerateSecPerFrameAccum += dt - FramerateSecPerFrame[FramerateSecPerFrameIdx];
    FramerateSecPerFrame[FramerateSecPerFrameIdx] = dt;
    FramerateSecPerFrameIdx = (FramerateSecPerFrameIdx + 1) % FramerateSecPerFrame.size();
    FramerateSecPerFrameCount = std::min(FramerateSecPerFrameCount + 1, FramerateSecPerFrame.size());
    return (FramerateSecPerFrameAccum > 0.0f) ? (1.0f / (FramerateSecPerFrameAccum / (float)FramerateSecPerFrameCount)) : FLT_MAX;
}
