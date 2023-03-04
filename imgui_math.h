#pragma once

#include <cmath>

/******************************************************************/

struct ImVec2
{
    float x = 0.0f;
    float y = 0.0f;

    float operator[] (size_t idx) const { return (&x)[idx]; }
    float& operator[] (size_t idx) { return (&x)[idx]; }

    bool operator==(const ImVec2& rhs) const = default;
    bool operator!=(const ImVec2& rhs) const = default;
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

inline ImVec2 Normalized(ImVec2 vec) {
    const float len = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    if (len > 0.0f) {
        vec /= len;
    }
    return vec;
}

constexpr ImVec2 ImMin(const ImVec2& lhs, const ImVec2& rhs) {
    return { std::min(lhs.x, rhs.x), std::min(lhs.y, rhs.y) };
}
constexpr ImVec2 ImMax(const ImVec2& lhs, const ImVec2& rhs) {
    return { std::max(lhs.x, rhs.x), std::max(lhs.y, rhs.y) };
}
constexpr ImVec2 ImClamp(const ImVec2& v, const ImVec2& mn, ImVec2 mx) {
    return ImVec2((v.x < mn.x) ? mn.x : (v.x > mx.x) ? mx.x : v.x, (v.y < mn.y) ? mn.y : (v.y > mx.y) ? mx.y : v.y);
}
constexpr ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, float t) {
    return ImVec2(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t);
}
constexpr ImVec2 ImLerp(const ImVec2& a, const ImVec2& b, const ImVec2& t) {
    return ImVec2(a.x + (b.x - a.x) * t.x, a.y + (b.y - a.y) * t.y);
}
inline ImVec2 ImFloor(const ImVec2& v) {
    return ImVec2((float)(int)(v.x), (float)(int)(v.y));
}

/******************************************************************/

struct ImVec4
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    bool operator==(const ImVec4& rhs) const = default;
    bool operator!=(const ImVec4& rhs) const = default;
};

constexpr ImVec4 operator+(const ImVec4& lhs, const ImVec4& rhs) {
    return ImVec4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w);
}
constexpr ImVec4 operator-(const ImVec4& lhs, const ImVec4& rhs) {
    return ImVec4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w);
}
constexpr ImVec4 operator*(const ImVec4& lhs, const ImVec4& rhs) {
    return ImVec4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w);
}

constexpr ImVec4 ImLerp(const ImVec4& a, const ImVec4& b, float t) {
    return {
        a.x + (b.x - a.x) * t,
        a.y + (b.y - a.y) * t,
        a.z + (b.z - a.z) * t,
        a.w + (b.w - a.w) * t
    };
}

/******************************************************************/

struct ImRect
{
    ImVec2 Min;    // Upper-left
    ImVec2 Max;    // Lower-right

    constexpr ImRect() = default;
    constexpr ImRect(ImVec2 min, ImVec2 max)
        : Min(min), Max(max)
    {}
    constexpr ImRect(const ImVec4& v)
        : Min(v.x, v.y)
        , Max(v.z, v.w)
    {}
    constexpr ImRect(float x1, float y1, float x2, float y2)
        : Min(x1, y1)
        , Max(x2, y2)
    {}
   
    ImVec2 GetSize() const { return ImVec2(Max.x - Min.x, Max.y - Min.y); }
    float GetWidth() const { return Max.x - Min.x; }
    float GetHeight() const { return Max.y - Min.y; }
    float GetArea() const { return (Max.x - Min.x) * (Max.y - Min.y); }

    ImVec2 GetCenter() const { return ImVec2((Min.x + Max.x) * 0.5f, (Min.y + Max.y) * 0.5f); }
    ImVec2 GetTL() const { return Min; }                   // Top-left
    ImVec2 GetTR() const { return ImVec2(Max.x, Min.y); }  // Top-right
    ImVec2 GetBL() const { return ImVec2(Min.x, Max.y); }  // Bottom-left
    ImVec2 GetBR() const { return Max; }                   // Bottom-right

    bool Contains(ImVec2 p) const {
        return p.x >= Min.x
            && p.y >= Min.y
            && p.x < Max.x
            && p.y < Max.y;
    }
    bool Contains(const ImRect& r) const {
        return r.Min.x >= Min.x
            && r.Min.y >= Min.y
            && r.Max.x <= Max.x
            && r.Max.y <= Max.y;
    }
    bool Overlaps(const ImRect& r) const {
        return r.Min.y < Max.y
            && r.Max.y > Min.y
            && r.Min.x < Max.x
            && r.Max.x > Min.x;
    }
    void Add(ImVec2 p) {
        if (Min.x > p.x) Min.x = p.x;
        if (Min.y > p.y) Min.y = p.y;
        if (Max.x < p.x) Max.x = p.x;
        if (Max.y < p.y) Max.y = p.y;
    }
    void Add(const ImRect& r) {
        if (Min.x > r.Min.x) Min.x = r.Min.x;
        if (Min.y > r.Min.y) Min.y = r.Min.y;
        if (Max.x < r.Max.x) Max.x = r.Max.x;
        if (Max.y < r.Max.y) Max.y = r.Max.y;
    }
    void Expand(float amount) {
        Min.x -= amount;
        Min.y -= amount;
        Max.x += amount;
        Max.y += amount;
    }
    void Expand(ImVec2 amount) {
        Min -= amount;
        Max += amount;
    }
    void Translate(ImVec2 offset) {
        Min += offset;
        Max += offset;
    }
    void TranslateX(float dx) {
        Min.x += dx;
        Max.x += dx;
    }
    void TranslateY(float dy) {
        Min.y += dy;
        Max.y += dy;
    }
    void ClipWith(const ImRect& r) {
        // Simple version, may lead to an inverted rectangle,
        // which is fine for Contains/Overlaps test but not for display.
        Min = ImMax(Min, r.Min);
        Max = ImMin(Max, r.Max);
    }
    void ClipWithFull(const ImRect& r) {
        // Full version, ensure both points are fully clipped.
        Min = ImClamp(Min, r.Min, r.Max);
        Max = ImClamp(Max, r.Min, r.Max);
    } 
    void Floor() {
        Min.x = std::floor(Min.x);
        Min.y = std::floor(Min.y);
        Max.x = std::floor(Max.x);
        Max.y = std::floor(Max.y);
    }
    bool IsInverted() const  { return Min.x > Max.x || Min.y > Max.y; }
    ImVec4 ToVec4() const { return ImVec4(Min.x, Min.y, Max.x, Max.y); }
};
