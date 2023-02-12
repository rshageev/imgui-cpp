#pragma once

#include <cstdint>

// Scalar data types
using ImGuiID = unsigned int; // A unique ID used by widgets (typically the result of hashing a stack of string)
using ImS8 = std::int8_t;     // 8-bit signed integer
using ImU8 = std::uint8_t;    // 8-bit unsigned integer
using ImS16 = std::int16_t;   // 16-bit signed integer
using ImU16 = std::uint16_t;  // 16-bit unsigned integer
using ImS32 = std::int32_t;   // 32-bit signed integer == int
using ImU32 = std::uint32_t;  // 32-bit unsigned integer (often used to store packed colors)
using ImS64 = std::int64_t;   // 64-bit signed integer
using ImU64 = std::uint64_t;  // 64-bit unsigned integer

// Character types
// (we generally use UTF-8 encoded string in the API. This is storage
// specifically for a decoded character used for keyboard input and display)
using ImWchar16 = std::uint16_t;   // A single decoded U16 character/code point. We encode them as multi bytes UTF-8 when used in strings.
using ImWchar32 = std::uint32_t;     // A single decoded U32 character/code point. We encode them as multi bytes UTF-8 when used in strings.
#ifdef IMGUI_USE_WCHAR32            // ImWchar [configurable type: override in imconfig.h with '#define IMGUI_USE_WCHAR32' to support Unicode planes 1-16]
using ImWchar = ImWchar32;
#else
using ImWchar = ImWchar16;
#endif


// ImVec2: 2D vector used to store positions, sizes etc. [Compile-time configurable type]
// This is a frequently used type in the API.
struct ImVec2
{
    float x = 0.0f;
    float y = 0.0f;

    float operator[] (size_t idx) const { return (&x)[idx]; }
    float& operator[] (size_t idx) { return (&x)[idx]; }

    bool operator==(const ImVec2& rhs) const = default;
    bool operator!=(const ImVec2& rhs) const = default;
};

// ImVec4: 4D vector used to store clipping rectangles, colors etc. [Compile-time configurable type]
struct ImVec4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    bool operator==(const ImVec4& rhs) const = default;
    bool operator!=(const ImVec4& rhs) const = default;
};

constexpr ImVec2 operator*(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x * rhs, lhs.y * rhs); }
constexpr ImVec2 operator/(const ImVec2& lhs, const float rhs) { return ImVec2(lhs.x / rhs, lhs.y / rhs); }
constexpr ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); }
constexpr ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
constexpr ImVec2 operator*(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y); }
constexpr ImVec2 operator/(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x / rhs.x, lhs.y / rhs.y); }
constexpr ImVec2& operator*=(ImVec2& lhs, const float rhs) { lhs.x *= rhs; lhs.y *= rhs; return lhs; }
constexpr ImVec2& operator/=(ImVec2& lhs, const float rhs) { lhs.x /= rhs; lhs.y /= rhs; return lhs; }
constexpr ImVec2& operator+=(ImVec2& lhs, const ImVec2& rhs) { lhs.x += rhs.x; lhs.y += rhs.y; return lhs; }
constexpr ImVec2& operator-=(ImVec2& lhs, const ImVec2& rhs) { lhs.x -= rhs.x; lhs.y -= rhs.y; return lhs; }
constexpr ImVec2& operator*=(ImVec2& lhs, const ImVec2& rhs) { lhs.x *= rhs.x; lhs.y *= rhs.y; return lhs; }
constexpr ImVec2& operator/=(ImVec2& lhs, const ImVec2& rhs) { lhs.x /= rhs.x; lhs.y /= rhs.y; return lhs; }
constexpr ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
constexpr ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
constexpr ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs) { return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }