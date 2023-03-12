#pragma once

#include <cstdint>
#include <cmath>

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
using ImWchar = char32_t;

// A cardinal direction
using ImGuiDir = int;
enum ImGuiDir_
{
    ImGuiDir_None    = -1,
    ImGuiDir_Left    = 0,
    ImGuiDir_Right   = 1,
    ImGuiDir_Up      = 2,
    ImGuiDir_Down    = 3,
    ImGuiDir_COUNT
};
