#include "imgui_color.h"

#include <cmath>

ImColorf ImColorf::FromHSV(float h, float s, float v, float a)
{
    ImColorf col;
    ImGui::ColorConvertHSVtoRGB(h, s, v, col.r, col.g, col.b);
    col.a = a;
    return col;
}

ImCol ImAlphaBlendColors(ImCol col_a, ImCol col_b)
{
    const auto t = static_cast<float>(col_b.a) / 255.0f;
    const auto r = static_cast<std::uint8_t>(std::lerp(col_a.r, col_b.r, t));
    const auto g = static_cast<std::uint8_t>(std::lerp(col_a.g, col_b.g, t));
    const auto b = static_cast<std::uint8_t>(std::lerp(col_a.b, col_b.b, t));
    return ImCol(r, g, b, 255);
}

namespace ImGui
{

// Convert rgb floats ([0-1],[0-1],[0-1]) to hsv floats ([0-1],[0-1],[0-1]), from Foley & van Dam p592
// Optimized http://lolengine.net/blog/2013/01/13/fast-rgb-to-hsv
void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v)
{
    float K = 0.f;
    if (g < b)
    {
        std::swap(g, b);
        K = -1.f;
    }
    if (r < g)
    {
        std::swap(r, g);
        K = -2.f / 6.f - K;
    }

    const float chroma = r - (g < b ? g : b);
    out_h = std::abs(K + (g - b) / (6.f * chroma + 1e-20f));
    out_s = chroma / (r + 1e-20f);
    out_v = r;
}

// Convert hsv floats ([0-1],[0-1],[0-1]) to rgb floats ([0-1],[0-1],[0-1]), from Foley & van Dam p593
// also http://en.wikipedia.org/wiki/HSL_and_HSV
void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b)
{
    if (s == 0.0f)
    {
        // gray
        out_r = out_g = out_b = v;
        return;
    }

    h = std::fmod(h, 1.0f) / (60.0f / 360.0f);
    int   i = (int)h;
    float f = h - (float)i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (i)
    {
    case 0: out_r = v; out_g = t; out_b = p; break;
    case 1: out_r = q; out_g = v; out_b = p; break;
    case 2: out_r = p; out_g = v; out_b = t; break;
    case 3: out_r = p; out_g = q; out_b = v; break;
    case 4: out_r = t; out_g = p; out_b = v; break;
    case 5: default: out_r = v; out_g = p; out_b = q; break;
    }
}

}
