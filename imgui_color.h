#pragma once

#include <cstdint>
#include <bit>

struct ImCol
{
    std::uint8_t r = 0;
    std::uint8_t g = 0;
    std::uint8_t b = 0;
    std::uint8_t a = 0;

    constexpr ImCol() = default;

    constexpr ImCol(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
        : r(red), g(green), b(blue), a(alpha)
    {}

    static ImCol FromU32(std::uint32_t col) {
        return std::bit_cast<ImCol>(col);
    }
    static std::uint32_t ToU32(ImCol col) {
        return std::bit_cast<std::uint32_t>(col);
    }

    ImCol WithAlpha(std::uint8_t alpha) const { return ImCol(r, g, b, alpha); }

    bool operator==(const ImCol&) const = default;

    static const ImCol White;
    static const ImCol Black;
    static const ImCol BlackTransp;
};
inline constexpr ImCol ImCol::White = ImCol(255, 255, 255, 255);
inline constexpr ImCol ImCol::Black = ImCol(0, 0, 0, 255);
inline constexpr ImCol ImCol::BlackTransp = ImCol(0, 0, 0, 0);


struct ImColorf
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;

    static ImColorf FromHSV(float h, float s, float v, float a = 1.0f);

    bool operator==(const ImColorf&) const = default;

    static const ImColorf White;
    static const ImColorf BlackTransparent;
};
inline constexpr ImColorf ImColorf::White = { 1.0f, 1.0f, 1.0f, 1.0f };
inline constexpr ImColorf ImColorf::BlackTransparent = { 0.0f, 0.0f, 0.0f, 0.0f };

ImCol ImAlphaBlendColors(ImCol col_a, ImCol col_b);

namespace ImGui
{
    void ColorConvertRGBtoHSV(float r, float g, float b, float& out_h, float& out_s, float& out_v);
    void ColorConvertHSVtoRGB(float h, float s, float v, float& out_r, float& out_g, float& out_b);

    constexpr float ColorComponentToFloat(std::uint8_t col) {
        return static_cast<float>(col / 255.0f);
    }
    constexpr std::uint8_t ColorComponentToByte(float col) {
        return static_cast<std::uint8_t>(col * 255.0f + 0.5f);
    }

    constexpr ImColorf ColorConvertToFloat(ImCol col)
    {
        return {
            ColorComponentToFloat(col.r),
            ColorComponentToFloat(col.g),
            ColorComponentToFloat(col.b),
            ColorComponentToFloat(col.a),
        };
    }

    constexpr ImCol ColorConvertToByte(ImColorf col)
    {
        return ImCol( ColorComponentToByte(col.r),
            ColorComponentToByte(col.g),
            ColorComponentToByte(col.b),
            ColorComponentToByte(col.a));
    }
}
