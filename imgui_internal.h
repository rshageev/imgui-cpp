// dear imgui, v1.89.3 WIP
// (internal structures/api)

#pragma once
#ifndef IMGUI_DISABLE

//-----------------------------------------------------------------------------
// [SECTION] Header mess
//-----------------------------------------------------------------------------

#ifndef IMGUI_VERSION
#include "imgui.h"
#endif
#include "utils/fps_counter.h"

#include <stdio.h>      // FILE*, sscanf
#include <stdlib.h>     // NULL, malloc, free, qsort, atoi, atof
#include <math.h>       // sqrtf, fabsf, fmodf, powf, floorf, ceilf, cosf, sinf
#include <limits.h>     // INT_MIN, INT_MAX

#include <algorithm>
#include <ranges>
#include <map>
#include <format>

namespace stdr = std::ranges;
namespace stdv = std::views;

// Visual Studio warnings
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable: 4251)     // class 'xxx' needs to have dll-interface to be used by clients of struct 'xxx' // when IMGUI_API is set to__declspec(dllexport)
#pragma warning (disable: 26812)    // The enum type 'xxx' is unscoped. Prefer 'enum class' over 'enum' (Enum.3). [MSVC Static Analyzer)
#if defined(_MSC_VER) && _MSC_VER >= 1922 // MSVC 2019 16.2 or later
#pragma warning (disable: 5054)     // operator '|': deprecated between enumerations of different types
#endif
#endif

// Clang/GCC warnings with -Weverything
#if defined(__clang__)
#pragma clang diagnostic push
#if __has_warning("-Wunknown-warning-option")
#pragma clang diagnostic ignored "-Wunknown-warning-option"         // warning: unknown warning group 'xxx'
#endif
#pragma clang diagnostic ignored "-Wunknown-pragmas"                // warning: unknown warning group 'xxx'
#pragma clang diagnostic ignored "-Wfloat-equal"                    // warning: comparing floating point with == or != is unsafe // storing and comparing against same constants ok, for ImFloorSigned()
#pragma clang diagnostic ignored "-Wunused-function"                // for stb_textedit.h
#pragma clang diagnostic ignored "-Wmissing-prototypes"             // for stb_textedit.h
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"  // warning: implicit conversion from 'xxx' to 'float' may lose precision
#pragma clang diagnostic ignored "-Wmissing-noreturn"               // warning: function 'xxx' could be declared with attribute 'noreturn'
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpragmas"              // warning: unknown option after '#pragma GCC diagnostic' kind
#pragma GCC diagnostic ignored "-Wclass-memaccess"      // [__GNUC__ >= 8] warning: 'memset/memcpy' clearing/writing an object of type 'xxxx' with no trivial copy-assignment; use assignment or value-initialization instead
#endif

// Enable stb_truetype by default unless FreeType is enabled.
// You can compile with both by defining both IMGUI_ENABLE_FREETYPE and IMGUI_ENABLE_STB_TRUETYPE together.
#ifndef IMGUI_ENABLE_FREETYPE
#define IMGUI_ENABLE_STB_TRUETYPE
#endif

//-----------------------------------------------------------------------------
// [SECTION] Forward declarations
//-----------------------------------------------------------------------------

struct ImRect;                      // An axis-aligned rectangle (2 points)
struct ImDrawDataBuilder;           // Helper to build a ImDrawData instance
struct ImDrawListSharedData;        // Data shared between all ImDrawList instances
struct ImGuiContext;                // Main Dear ImGui context
struct ImGuiContextHook;            // Hook for extensions like ImGuiTestEngine
struct ImGuiDataTypeInfo;           // Type information associated to a ImGuiDataType enum
struct ImGuiGroupData;              // Stacked storage data for BeginGroup()/EndGroup()
struct ImGuiInputTextState;         // Internal state of the currently focused/edited text input box
struct ImGuiLastItemData;           // Status storage for last submitted items
struct ImGuiLocEntry;               // A localization entry.
struct ImGuiMenuColumns;            // Simple column measurement, currently used for MenuItem() only
struct ImGuiNavItemData;            // Result of a gamepad/keyboard directional navigation move query result
struct ImGuiMetricsConfig;          // Storage for ShowMetricsWindow() and DebugNodeXXX() functions
struct ImGuiNextWindowData;         // Storage for SetNextWindow** functions
struct ImGuiNextItemData;           // Storage for SetNextItem** functions
struct ImGuiOldColumnData;          // Storage data for a single column for legacy Columns() api
struct ImGuiOldColumns;             // Storage data for a columns set for legacy Columns() api
struct ImGuiPopupData;              // Storage for current popup stack
struct ImGuiSettingsHandler;        // Storage for one type registered in the .ini file
struct ImGuiStyleMod;               // Stacked style modifier, backup of modified data so we can restore it
struct ImGuiTabBar;                 // Storage for a tab bar
struct ImGuiTabItem;                // Storage for a tab item (within a tab bar)
struct ImGuiTable;                  // Storage for a table
struct ImGuiTableColumn;            // Storage for one column of a table
struct ImGuiTableInstanceData;      // Storage for one instance of a same table
struct ImGuiTableTempData;          // Temporary storage for one table (one per table in the stack), shared between tables.
struct ImGuiTableSettings;          // Storage for a table .ini settings
struct ImGuiTableColumnsSettings;   // Storage for a column .ini settings
struct ImGuiWindow;                 // Storage for one window
struct ImGuiWindowTempData;         // Temporary storage for one window (that's the data which in theory we could ditch at the end of the frame, in practice we currently keep it for each window)
struct ImGuiWindowSettings;         // Storage for a window .ini settings (we keep one of those even if the actual window wasn't instanced during this session)

// Enumerations
// Use your programming IDE "Go to definition" facility on the names of the center columns to find the actual flags/enum lists.
enum ImGuiLocKey : int;                 // -> enum ImGuiLocKey              // Enum: a localization entry for translation.
typedef int ImGuiLayoutType;            // -> enum ImGuiLayoutType_         // Enum: Horizontal or vertical

// Flags
typedef int ImGuiActivateFlags;         // -> enum ImGuiActivateFlags_      // Flags: for navigation/focus function (will be for ActivateItem() later)
typedef int ImGuiDebugLogFlags;         // -> enum ImGuiDebugLogFlags_      // Flags: for ShowDebugLogWindow(), g.DebugLogFlags
typedef int ImGuiInputFlags;            // -> enum ImGuiInputFlags_         // Flags: for IsKeyPressed(), IsMouseClicked(), SetKeyOwner(), SetItemKeyOwner() etc.
typedef int ImGuiItemFlags;             // -> enum ImGuiItemFlags_          // Flags: for PushItemFlag(), g.LastItemData.InFlags
typedef int ImGuiItemStatusFlags;       // -> enum ImGuiItemStatusFlags_    // Flags: for g.LastItemData.StatusFlags
typedef int ImGuiNavHighlightFlags;     // -> enum ImGuiNavHighlightFlags_  // Flags: for RenderNavHighlight()
typedef int ImGuiNavMoveFlags;          // -> enum ImGuiNavMoveFlags_       // Flags: for navigation requests
typedef int ImGuiNextItemDataFlags;     // -> enum ImGuiNextItemDataFlags_  // Flags: for SetNextItemXXX() functions
typedef int ImGuiNextWindowDataFlags;   // -> enum ImGuiNextWindowDataFlags_// Flags: for SetNextWindowXXX() functions
typedef int ImGuiScrollFlags;           // -> enum ImGuiScrollFlags_        // Flags: for ScrollToItem() and navigation requests
typedef int ImGuiSeparatorFlags;        // -> enum ImGuiSeparatorFlags_     // Flags: for SeparatorEx()
typedef int ImGuiTextFlags;             // -> enum ImGuiTextFlags_          // Flags: for TextEx()
typedef int ImGuiTooltipFlags;          // -> enum ImGuiTooltipFlags_       // Flags: for BeginTooltipEx()

typedef void (*ImGuiErrorLogCallback)(void* user_data, const char* fmt, ...);

//-----------------------------------------------------------------------------
// [SECTION] Context pointer
// See implementation of this variable in imgui.cpp for comments and details.
//-----------------------------------------------------------------------------

#ifndef GImGui
extern ImGuiContext* GImGui;  // Current implicit context pointer
#endif

//-------------------------------------------------------------------------
// [SECTION] STB libraries includes
//-------------------------------------------------------------------------

namespace ImStb
{

#undef STB_TEXTEDIT_STRING
#undef STB_TEXTEDIT_CHARTYPE
#define STB_TEXTEDIT_STRING             ImGuiInputTextState
#define STB_TEXTEDIT_CHARTYPE           ImWchar
#define STB_TEXTEDIT_GETWIDTH_NEWLINE   (-1.0f)
#define STB_TEXTEDIT_UNDOSTATECOUNT     99
#define STB_TEXTEDIT_UNDOCHARCOUNT      999
#include "imstb_textedit.h"

} // namespace ImStb

//-----------------------------------------------------------------------------
// [SECTION] Macros
//-----------------------------------------------------------------------------

// Debug Printing Into TTY
// (since IMGUI_VERSION_NUM >= 18729: IMGUI_DEBUG_LOG was reworked into IMGUI_DEBUG_PRINTF (and removed framecount from it). If you were using a #define IMGUI_DEBUG_LOG please rename)
#ifndef IMGUI_DEBUG_PRINTF
#define IMGUI_DEBUG_PRINTF(_FMT,...)    printf(_FMT, __VA_ARGS__)
#endif

// Debug Logging for ShowDebugLogWindow(). This is designed for relatively rare events so please don't spam.
#define IMGUI_DEBUG_LOG(...)            ImGui::DebugLog(__VA_ARGS__)
#define IMGUI_DEBUG_LOG_ACTIVEID(...)   do { if (g.DebugLogFlags & ImGuiDebugLogFlags_EventActiveId) IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_FOCUS(...)      do { if (g.DebugLogFlags & ImGuiDebugLogFlags_EventFocus)    IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_POPUP(...)      do { if (g.DebugLogFlags & ImGuiDebugLogFlags_EventPopup)    IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_NAV(...)        do { if (g.DebugLogFlags & ImGuiDebugLogFlags_EventNav)      IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_CLIPPER(...)    do { if (g.DebugLogFlags & ImGuiDebugLogFlags_EventClipper)  IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)
#define IMGUI_DEBUG_LOG_IO(...)         do { if (g.DebugLogFlags & ImGuiDebugLogFlags_EventIO)       IMGUI_DEBUG_LOG(__VA_ARGS__); } while (0)

// "Paranoid" Debug Asserts are meant to only be enabled during specific debugging/work, otherwise would slow down the code too much.
// We currently don't have many of those so the effect is currently negligible, but onward intent to add more aggressive ones in the code.
//#define IMGUI_DEBUG_PARANOID
#ifdef IMGUI_DEBUG_PARANOID
#define IM_ASSERT_PARANOID(_EXPR)       IM_ASSERT(_EXPR)
#else
#define IM_ASSERT_PARANOID(_EXPR)
#endif

// Error handling
// Down the line in some frameworks/languages we would like to have a way to redirect those to the programmer and recover from more faults.
#ifndef IM_ASSERT_USER_ERROR
#define IM_ASSERT_USER_ERROR(_EXP,_MSG) IM_ASSERT((_EXP) && _MSG)   // Recoverable User Error
#endif

// Misc Macros
#define IM_PI                           3.14159265358979323846f
#ifdef _WIN32
#define IM_NEWLINE                      "\r\n"   // Play it nice with Windows users (Update: since 2018-05, Notepad finally appears to support Unix-style carriage returns!)
#else
#define IM_NEWLINE                      "\n"
#endif
#ifndef IM_TABSIZE                      // Until we move this to runtime and/or add proper tab support, at least allow users to compile-time override
#define IM_TABSIZE                      (4)
#endif
#define IM_MEMALIGN(_OFF,_ALIGN)        (((_OFF) + ((_ALIGN) - 1)) & ~((_ALIGN) - 1))           // Memory align e.g. IM_ALIGN(0,4)=0, IM_ALIGN(1,4)=4, IM_ALIGN(4,4)=4, IM_ALIGN(5,4)=8
#define IM_F32_TO_INT8_UNBOUND(_VAL)    ((int)((_VAL) * 255.0f + ((_VAL)>=0 ? 0.5f : -0.5f)))   // Unsaturated, for display purpose
#define IM_F32_TO_INT8_SAT(_VAL)        ((int)(ImSaturate(_VAL) * 255.0f + 0.5f))               // Saturated, always output 0..255

// Warnings
#if defined(_MSC_VER) && !defined(__clang__)
#define IM_MSVC_WARNING_SUPPRESS(XXXX)  __pragma(warning(suppress: XXXX))
#else
#define IM_MSVC_WARNING_SUPPRESS(XXXX)
#endif

// Debug Tools
// Use 'Metrics/Debugger->Tools->Item Picker' to break into the call-stack of a specific item.
// This will call IM_DEBUG_BREAK() which you may redefine yourself. See https://github.com/scottt/debugbreak for more reference.
#ifndef IM_DEBUG_BREAK
#if defined (_MSC_VER)
#define IM_DEBUG_BREAK()    __debugbreak()
#elif defined(__clang__)
#define IM_DEBUG_BREAK()    __builtin_debugtrap()
#elif defined(__GNUC__) && (defined(__i386__) || defined(__x86_64__))
#define IM_DEBUG_BREAK()    __asm__ volatile("int $0x03")
#elif defined(__GNUC__) && defined(__thumb__)
#define IM_DEBUG_BREAK()    __asm__ volatile(".inst 0xde01")
#elif defined(__GNUC__) && defined(__arm__) && !defined(__thumb__)
#define IM_DEBUG_BREAK()    __asm__ volatile(".inst 0xe7f001f0");
#else
#define IM_DEBUG_BREAK()    IM_ASSERT(0)    // It is expected that you define IM_DEBUG_BREAK() into something that will break nicely in a debugger!
#endif
#endif // #ifndef IM_DEBUG_BREAK


// Helpers: Hashing
ImGuiID ImHashData(const void* data, size_t data_size, ImGuiID seed = 0);
ImGuiID ImHashStr(std::string_view str, ImGuiID seed = 0);

template<class T>
ImGuiID ImHash(T& data, ImGuiID seed = 0) {
    return ImHashData(std::addressof(data), sizeof(T), seed);
}

// Helpers: Bit manipulation
static inline bool      ImIsPowerOfTwo(int v)           { return v != 0 && (v & (v - 1)) == 0; }
static inline bool      ImIsPowerOfTwo(ImU64 v)         { return v != 0 && (v & (v - 1)) == 0; }
static inline int       ImUpperPowerOfTwo(int v)        { v--; v |= v >> 1; v |= v >> 2; v |= v >> 4; v |= v >> 8; v |= v >> 16; v++; return v; }

// Helpers: String
void          ImStrncpy(char* dst, const char* src, size_t count);
char*         ImStrdup(const char* str);
char*         ImStrdupcpy(char* dst, size_t* p_dst_size, const char* str);
const char*   ImStrchrRange(const char* str_begin, const char* str_end, char c);
int           ImStrlenW(const ImWchar* str);
const char*   ImStreolRange(const char* str, const char* str_end);                // End end-of-line
const ImWchar*ImStrbolW(const ImWchar* buf_mid_line, const ImWchar* buf_begin);   // Find beginning-of-line
void          ImStrTrimBlanks(char* str);
const char*   ImStrSkipBlank(const char* str);

inline char ImToUpper(char c) { return (c >= 'a' && c <= 'z') ? c &= ~32 : c; }
inline bool ImCharIsBlankA(char c) { return c == ' ' || c == '\t'; }
inline bool ImCharIsBlankW(unsigned int c){ return c == ' ' || c == '\t' || c == 0x3000; }

// Helpers: Formatting
int           ImFormatString(char* buf, size_t buf_size, const char* fmt, ...) IM_FMTARGS(3);
std::string ImFormatStringToString(const char* fmt, ...);
std::string ImFormatStringToStringV(const char* fmt, va_list args);
const char*   ImParseFormatFindStart(const char* format);
const char*   ImParseFormatFindEnd(const char* format);
const char*   ImParseFormatTrimDecorations(const char* format, char* buf, size_t buf_size);
void          ImParseFormatSanitizeForPrinting(const char* fmt_in, char* fmt_out, size_t fmt_out_size);
const char*   ImParseFormatSanitizeForScanning(const char* fmt_in, char* fmt_out, size_t fmt_out_size);
int           ImParseFormatPrecision(const char* format, int default_value);

// Helpers: UTF-8 <> wchar conversions
const char*   ImTextCharToUtf8(char out_buf[5], unsigned int c);                                                      // return out_buf
int           ImTextStrToUtf8(char* out_buf, int out_buf_size, const ImWchar* in_text, const ImWchar* in_text_end);   // return output UTF-8 bytes count
int           ImTextCharFromUtf8(unsigned int* out_char, const char* in_text, const char* in_text_end);               // read one character. return input UTF-8 bytes count
int           ImTextStrFromUtf8(ImWchar* out_buf, int out_buf_size, const char* in_text, const char* in_text_end, const char** in_remaining = NULL);   // return input UTF-8 bytes count
int           ImTextCountCharsFromUtf8(const char* in_text, const char* in_text_end);                                 // return number of UTF-8 code-points (NOT bytes count)
int           ImTextCountUtf8BytesFromChar(const char* in_text, const char* in_text_end);                             // return number of bytes to express one char in UTF-8
int           ImTextCountUtf8BytesFromStr(const ImWchar* in_text, const ImWchar* in_text_end);                        // return number of bytes to express string in UTF-8

// Helpers: File System
#ifdef IMGUI_DISABLE_FILE_FUNCTIONS
#define IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef void* ImFileHandle;
static inline ImFileHandle  ImFileOpen(const char*, const char*)                    { return NULL; }
static inline bool          ImFileClose(ImFileHandle)                               { return false; }
static inline ImU64         ImFileGetSize(ImFileHandle)                             { return (ImU64)-1; }
static inline ImU64         ImFileRead(void*, ImU64, ImU64, ImFileHandle)           { return 0; }
static inline ImU64         ImFileWrite(const void*, ImU64, ImU64, ImFileHandle)    { return 0; }
#endif
#ifndef IMGUI_DISABLE_DEFAULT_FILE_FUNCTIONS
typedef FILE* ImFileHandle;
ImFileHandle ImFileOpen(const char* filename, const char* mode);
bool ImFileClose(ImFileHandle file);
ImU64 ImFileGetSize(ImFileHandle file);
ImU64 ImFileRead(void* data, ImU64 size, ImU64 count, ImFileHandle file);
ImU64  ImFileWrite(const void* data, ImU64 size, ImU64 count, ImFileHandle file);
#else
#define IMGUI_DISABLE_TTY_FUNCTIONS // Can't use stdout, fflush if we are not using default file functions
#endif
void* ImFileLoadToMemory(const char* filename, const char* mode, size_t* out_file_size = NULL, int padding_bytes = 0);

// Helpers: Maths
// - Wrapper for standard libs functions. (Note that imgui_demo.cpp does _not_ use them to keep the code easy to copy)
#ifndef IMGUI_DISABLE_DEFAULT_MATH_FUNCTIONS
#define ImFmod(X, Y)        fmodf((X), (Y))
#define ImAtan2(Y, X)       atan2f((Y), (X))
#define ImCeil(X)           ceilf(X)
inline float  ImPow(float x, float y)    { return powf(x, y); }          // DragBehaviorT/SliderBehaviorT uses ImPow with either float/double and need the precision
inline double ImPow(double x, double y)  { return pow(x, y); }
inline float  ImSign(float x)            { return (x < 0.0f) ? -1.0f : (x > 0.0f) ? 1.0f : 0.0f; } // Sign operator - returns -1, 0 or 1 based on sign of argument
inline double ImSign(double x)           { return (x < 0.0) ? -1.0 : (x > 0.0) ? 1.0 : 0.0; }
inline float  ImRsqrt(float x)           { return 1.0f / sqrtf(x); }
inline double ImRsqrt(double x)          { return 1.0 / sqrt(x); }
#endif
// - ImMin/ImMax/ImClamp/ImLerp/ImSwap are used by widgets which support variety of types: signed/unsigned int/long long float/double
// (Exceptionally using templates here but we could also redefine them for those types)
template<typename T>
T ImMin(T lhs, T rhs) { return lhs < rhs ? lhs : rhs; }
template<typename T>
T ImMax(T lhs, T rhs) { return lhs >= rhs ? lhs : rhs; }
template<typename T>
T ImClamp(T v, T mn, T mx) { return (v < mn) ? mn : (v > mx) ? mx : v; }
template<typename T>
T ImLerp(T a, T b, float t) { return (T)(a + (b - a) * t); }

template<typename T>
T ImAddClampOverflow(T a, T b, T mn, T mx) {
    if (b < 0 && (a < mn - b)) return mn;
    if (b > 0 && (a > mx - b)) return mx;
    return a + b;
}
template<typename T>
T ImSubClampOverflow(T a, T b, T mn, T mx) {
    if (b > 0 && (a < mn + b)) return mn;
    if (b < 0 && (a > mx + b)) return mx;
    return a - b;
}

// - Misc maths helpers
inline float ImSaturate(float f) {
    return (f < 0.0f) ? 0.0f : (f > 1.0f) ? 1.0f : f;
}
inline float ImLengthSqr(const ImVec2& lhs) {
    return (lhs.x * lhs.x) + (lhs.y * lhs.y);
}
inline float ImLengthSqr(const ImVec4& lhs) {
    return (lhs.x * lhs.x) + (lhs.y * lhs.y) + (lhs.z * lhs.z) + (lhs.w * lhs.w);
}
inline float ImFloor(float f) {
    return (float)(int)(f);
}
inline float ImFloorSigned(float f) {
    return (float)((f >= 0 || (float)(int)f == f) ? (int)f : (int)f - 1); // Decent replacement for floorf()
} 
inline ImVec2 ImFloorSigned(const ImVec2& v) {
    return ImVec2(ImFloorSigned(v.x), ImFloorSigned(v.y));
}
inline float ImDot(const ImVec2& a, const ImVec2& b) {
    return a.x * b.x + a.y * b.y;
}
inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a) {
    return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}
inline float ImLinearSweep(float current, float target, float speed) {
    if (current < target)
        return ImMin(current + speed, target);
    if (current > target)
        return ImMax(current - speed, target);
    return current;
}
inline ImVec2 ImMul(const ImVec2& lhs, const ImVec2& rhs) {
    return ImVec2(lhs.x * rhs.x, lhs.y * rhs.y);
}
inline bool ImIsFloatAboveGuaranteedIntegerPrecision(float f) {
    return f <= -16777216 || f >= 16777216;
}
inline float ImExponentialMovingAverage(float avg, float sample, int n) {
    avg -= avg / n;
    avg += sample / n;
    return avg;
}

// Helpers: Geometry
ImVec2 ImBezierCubicCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, float t);
ImVec2 ImBezierCubicClosestPoint(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, int num_segments);       // For curves with explicit number of segments
ImVec2 ImBezierCubicClosestPointCasteljau(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, const ImVec2& p4, const ImVec2& p, float tess_tol);// For auto-tessellated curves you can use tess_tol = style.CurveTessellationTol
ImVec2 ImBezierQuadraticCalc(const ImVec2& p1, const ImVec2& p2, const ImVec2& p3, float t);
ImVec2 ImLineClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& p);
bool   ImTriangleContainsPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
ImVec2 ImTriangleClosestPoint(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p);
void   ImTriangleBarycentricCoords(const ImVec2& a, const ImVec2& b, const ImVec2& c, const ImVec2& p, float& out_u, float& out_v, float& out_w);
inline float ImTriangleArea(const ImVec2& a, const ImVec2& b, const ImVec2& c) {
    return std::abs((a.x * (b.y - c.y)) + (b.x * (c.y - a.y)) + (c.x * (a.y - b.y))) * 0.5f;
}
ImGuiDir ImGetDirQuadrantFromDelta(float dx, float dy);

// Helper: ImVec1 (1D vector)
// (this odd construct is used to facilitate the transition between 1D and 2D, and the maintenance of some branches/patches)
struct ImVec1
{
    float   x;
    constexpr ImVec1()         : x(0.0f) { }
    constexpr ImVec1(float _x) : x(_x) { }
};

// Helper: ImVec2ih (2D vector, half-size integer, for long-term packed storage)
struct ImVec2ih
{
    short   x, y;
    constexpr ImVec2ih()                           : x(0), y(0) {}
    constexpr ImVec2ih(short _x, short _y)         : x(_x), y(_y) {}
    constexpr explicit ImVec2ih(const ImVec2& rhs) : x((short)rhs.x), y((short)rhs.y) {}
};


// Helper: ImPool<>
// Basic keyed storage for contiguous instances, slow/amortized insertion, O(1) indexable, O(Log N) queries by ID over a dense/hot buffer,
// Honor constructor/destructor. Add/remove invalidate all pointers. Indexes have the same lifetime as the associated object.
template<typename T>
class ImPool
{
public:
    ImPool() = default;

    ~ImPool() { Clear(); }

    ImPool(const ImPool&) = delete;
    ImPool& operator=(const ImPool&) = delete;

    T* GetByKey(ImGuiID key) {
        auto itr = map.find(key);
        if (itr != map.end()) {
            return vec[itr->second].get();
        } else {
            return nullptr;
        }
    }

    T* GetByIndex(int n) { return vec[n].get(); }

    int GetIndex(const T* p) const {
        auto itr = stdr::find_if(vec, [p](auto& up){ return p == up.get(); });
        return (int)(itr - stdr::begin(vec));
    }

    T* GetOrAddByKey(ImGuiID key) {
        auto itr = map.find(key);
        if (itr != map.end()) {
            return vec[itr->second].get();
        } else {
            T* added = Add();
            map[key] = GetIndex(added);
            return added;
        }
    }

    bool Contains(const T* p) const {
        return stdr::find_if(vec, [p](auto& up){ return p == up.get(); }) != stdr::end(vec);
    }

    void Clear() {
        map.clear();
        vec.clear();
        AliveCount = 0;
    }

    void Remove(ImGuiID key, const T* p) {
        Remove(key, GetIndex(p));
    }

    void Remove(ImGuiID key, int idx) {
        vec[idx].reset();
        map.erase(key);
        AliveCount--;
    }

    void Reserve(int capacity) {
        vec.reserve(capacity);
    }

    int GetAliveCount() const {
        return (int)stdr::count_if(vec, [](auto& up){ return up != nullptr; });
    }

    auto Iterate()
    {
        return vec
            | stdv::filter([](auto& uptr){ return uptr != nullptr; }) // skipp null elements
            | stdv::transform([](auto& uptr){ return uptr.get(); });  // convert to raw pointers
    }

private:
    std::vector<std::unique_ptr<T>> vec; // Contiguous data
    std::map<ImGuiID, size_t> map;
    int AliveCount = 0; // Number of active/alive items (for display purpose)

    T* Add() {
        T* elem = new T();
        auto itr = stdr::find_if(vec, [](auto& up){ return up == nullptr; });
        if (itr == stdr::end(vec)) {
            vec.emplace_back(elem);
        } else {
            itr->reset(elem);
        }
        AliveCount++;
        return elem;
    }
};

//-----------------------------------------------------------------------------
// [SECTION] ImDrawList support
//-----------------------------------------------------------------------------

// ImDrawList: Helper function to calculate a circle's segment count given its radius and a "maximum error" value.
// Estimation of number of circle segment based on error is derived using method described in https://stackoverflow.com/a/2244088/15194693
// Number of segments (N) is calculated using equation:
//   N = ceil ( pi / acos(1 - error / r) )     where r > 0, error <= r
// Our equation is significantly simpler that one in the post thanks for choosing segment that is
// perpendicular to X axis. Follow steps in the article from this starting condition and you will
// will get this result.
//
// Rendering circles with an odd number of segments, while mathematically correct will produce
// asymmetrical results on the raster grid. Therefore we're rounding N to next even number (7->8, 8->8, 9->10 etc.)
#define IM_ROUNDUP_TO_EVEN(_V)                                  ((((_V) + 1) / 2) * 2)
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN                     4
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX                     512
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC(_RAD,_MAXERROR)    ImClamp(IM_ROUNDUP_TO_EVEN((int)ImCeil(IM_PI / std::acos(1 - ImMin((_MAXERROR), (_RAD)) / (_RAD)))), IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MIN, IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_MAX)

// Raw equation from IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC rewritten for 'r' and 'error'.
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_R(_N,_MAXERROR)    ((_MAXERROR) / (1 - std::cos(IM_PI / ImMax((float)(_N), IM_PI))))
#define IM_DRAWLIST_CIRCLE_AUTO_SEGMENT_CALC_ERROR(_N,_RAD)     ((1 - std::cos(IM_PI / ImMax((float)(_N), IM_PI))) / (_RAD))

// ImDrawList: Lookup table size for adaptive arc drawing, cover full circle.
#ifndef IM_DRAWLIST_ARCFAST_TABLE_SIZE
#define IM_DRAWLIST_ARCFAST_TABLE_SIZE                          48 // Number of samples in lookup table.
#endif
#define IM_DRAWLIST_ARCFAST_SAMPLE_MAX                          IM_DRAWLIST_ARCFAST_TABLE_SIZE // Sample index _PathArcToFastEx() for 360 angle.

// Data shared between all ImDrawList instances
// You may want to create your own instance of this if you want to use ImDrawList completely without ImGui. In that case, watch out for future changes to this structure.
struct ImDrawListSharedData
{
    ImVec2 TexUvWhitePixel;                              // UV of white pixel in the atlas
    ImFont* Font = nullptr;                              // Current/default font (optional, for simplified AddText overload)
    float FontSize = 0.0f;                               // Current/default font size (optional, for simplified AddText overload)
    float CurveTessellationTol = 0.0f;                   // Tessellation tolerance when using PathBezierCurveTo()
    float CircleSegmentMaxError = 0.0f;                  // Number of circle segments to use per pixel of radius for AddCircle() etc
    ImVec4 ClipRectFullscreen;                           // Value for PushClipRectFullscreen()
    ImDrawListFlags InitialFlags = ImDrawListFlags_None; // Initial flags at the beginning of the frame (it is possible to alter flags on a per-drawlist basis afterwards)

    // [Internal] Lookup tables
    ImVec2 ArcFastVtx[IM_DRAWLIST_ARCFAST_TABLE_SIZE];   // Sample points on the quarter of the circle.
    float ArcFastRadiusCutoff = 0.0f;                    // Cutoff radius after which arc drawing will fallback to slower PathArcTo()
    ImU8 CircleSegmentCounts[64] {0};                    // Precomputed segment count for given radius before we calculate it dynamically (to avoid calculation overhead)
    const ImVec4* TexUvLines = nullptr;                  // UV of anti-aliased lines in the atlas

    ImDrawListSharedData();
    void SetCircleTessellationMaxError(float max_error);
};

struct ImDrawDataBuilder
{
    std::vector<ImDrawList*> Layers[2]; // Global layers for: regular, tooltip

    void Clear() {
        for (auto& layer : Layers) {
            layer.clear();
        }
    }

    size_t GetDrawListCount() const {
        size_t count = 0;
        for (const auto& layer : Layers)
            count += layer.size();
        return count;
    }

    void FlattenIntoSingleLayer();
};

//-----------------------------------------------------------------------------
// [SECTION] Widgets support: flags, enums, data structures
//-----------------------------------------------------------------------------

// Flags used by upcoming items
// - input: PushItemFlag() manipulates g.CurrentItemFlags, ItemAdd() calls may add extra flags.
// - output: stored in g.LastItemData.InFlags
// Current window shared by all windows.
// This is going to be exposed in imgui.h when stabilized enough.
enum ImGuiItemFlags_
{
    // Controlled by user
    ImGuiItemFlags_None                     = 0,
    ImGuiItemFlags_NoTabStop                = 1 << 0,  // false     // Disable keyboard tabbing (FIXME: should merge with _NoNav)
    ImGuiItemFlags_ButtonRepeat             = 1 << 1,  // false     // Button() will return true multiple times based on io.KeyRepeatDelay and io.KeyRepeatRate settings.
    ImGuiItemFlags_Disabled                 = 1 << 2,  // false     // Disable interactions but doesn't affect visuals. See BeginDisabled()/EndDisabled(). See github.com/ocornut/imgui/issues/211
    ImGuiItemFlags_NoNav                    = 1 << 3,  // false     // Disable keyboard/gamepad directional navigation (FIXME: should merge with _NoTabStop)
    ImGuiItemFlags_NoNavDefaultFocus        = 1 << 4,  // false     // Disable item being a candidate for default focus (e.g. used by title bar items)
    ImGuiItemFlags_SelectableDontClosePopup = 1 << 5,  // false     // Disable MenuItem/Selectable() automatically closing their popup window
    ImGuiItemFlags_MixedValue               = 1 << 6,  // false     // [BETA] Represent a mixed/indeterminate value, generally multi-selection where values differ. Currently only supported by Checkbox() (later should support all sorts of widgets)
    ImGuiItemFlags_ReadOnly                 = 1 << 7,  // false     // [ALPHA] Allow hovering interactions but underlying value is not changed.
    ImGuiItemFlags_NoWindowHoverableCheck   = 1 << 8,  // false     // Disable hoverable check in ItemHoverable()

    // Controlled by widget code
    ImGuiItemFlags_Inputable                = 1 << 10, // false     // [WIP] Auto-activate input mode when tab focused. Currently only used and supported by a few items before it becomes a generic feature.
};

// Status flags for an already submitted item
// - output: stored in g.LastItemData.StatusFlags
enum ImGuiItemStatusFlags_
{
    ImGuiItemStatusFlags_None               = 0,
    ImGuiItemStatusFlags_HoveredRect        = 1 << 0,   // Mouse position is within item rectangle (does NOT mean that the window is in correct z-order and can be hovered!, this is only one part of the most-common IsItemHovered test)
    ImGuiItemStatusFlags_HasDisplayRect     = 1 << 1,   // g.LastItemData.DisplayRect is valid
    ImGuiItemStatusFlags_Edited             = 1 << 2,   // Value exposed by item was edited in the current frame (should match the bool return value of most widgets)
    ImGuiItemStatusFlags_ToggledSelection   = 1 << 3,   // Set when Selectable(), TreeNode() reports toggling a selection. We can't report "Selected", only state changes, in order to easily handle clipping with less issues.
    ImGuiItemStatusFlags_ToggledOpen        = 1 << 4,   // Set when TreeNode() reports toggling their open state.
    ImGuiItemStatusFlags_HasDeactivated     = 1 << 5,   // Set if the widget/group is able to provide data for the ImGuiItemStatusFlags_Deactivated flag.
    ImGuiItemStatusFlags_Deactivated        = 1 << 6,   // Only valid if ImGuiItemStatusFlags_HasDeactivated is set.
    ImGuiItemStatusFlags_HoveredWindow      = 1 << 7,   // Override the HoveredWindow test to allow cross-window hover testing.
    ImGuiItemStatusFlags_FocusedByTabbing   = 1 << 8,   // Set when the Focusable item just got focused by Tabbing (FIXME: to be removed soon)
    ImGuiItemStatusFlags_Visible            = 1 << 9,   // [WIP] Set when item is overlapping the current clipping rectangle (Used internally. Please don't use yet: API/system will change as we refactor Itemadd()).
};

// Extend ImGuiInputTextFlags_
enum ImGuiInputTextFlagsPrivate_
{
    // [Internal]
    ImGuiInputTextFlags_Multiline           = 1 << 26,  // For internal use by InputTextMultiline()
    ImGuiInputTextFlags_NoMarkEdited        = 1 << 27,  // For internal use by functions using InputText() before reformatting data
    ImGuiInputTextFlags_MergedItem          = 1 << 28,  // For internal use by TempInputText(), will skip calling ItemAdd(). Require bounding-box to strictly match.
};

// Extend ImGuiButtonFlags_
enum ImGuiButtonFlagsPrivate_
{
    ImGuiButtonFlags_PressedOnClick         = 1 << 4,   // return true on click (mouse down event)
    ImGuiButtonFlags_PressedOnClickRelease  = 1 << 5,   // [Default] return true on click + release on same item <-- this is what the majority of Button are using
    ImGuiButtonFlags_PressedOnClickReleaseAnywhere = 1 << 6, // return true on click + release even if the release event is not done while hovering the item
    ImGuiButtonFlags_PressedOnRelease       = 1 << 7,   // return true on release (default requires click+release)
    ImGuiButtonFlags_PressedOnDoubleClick   = 1 << 8,   // return true on double-click (default requires click+release)
    ImGuiButtonFlags_PressedOnDragDropHold  = 1 << 9,   // return true when held into while we are drag and dropping another item (used by e.g. tree nodes, collapsing headers)
    ImGuiButtonFlags_Repeat                 = 1 << 10,  // hold to repeat
    ImGuiButtonFlags_FlattenChildren        = 1 << 11,  // allow interactions even if a child window is overlapping
    ImGuiButtonFlags_AllowItemOverlap       = 1 << 12,  // require previous frame HoveredId to either match id or be null before being usable, use along with SetItemAllowOverlap()
    ImGuiButtonFlags_DontClosePopups        = 1 << 13,  // disable automatically closing parent popup on press // [UNUSED]
    //ImGuiButtonFlags_Disabled             = 1 << 14,  // disable interactions -> use BeginDisabled() or ImGuiItemFlags_Disabled
    ImGuiButtonFlags_AlignTextBaseLine      = 1 << 15,  // vertically align button to match text baseline - ButtonEx() only // FIXME: Should be removed and handled by SmallButton(), not possible currently because of DC.CursorPosPrevLine
    ImGuiButtonFlags_NoKeyModifiers         = 1 << 16,  // disable mouse interaction if a key modifier is held
    ImGuiButtonFlags_NoHoldingActiveId      = 1 << 17,  // don't set ActiveId while holding the mouse (ImGuiButtonFlags_PressedOnClick only)
    ImGuiButtonFlags_NoNavFocus             = 1 << 18,  // don't override navigation focus when activated (FIXME: this is essentially used everytime an item uses ImGuiItemFlags_NoNav, but because legacy specs don't requires LastItemData to be set ButtonBehavior(), we can't poll g.LastItemData.InFlags)
    ImGuiButtonFlags_NoHoveredOnFocus       = 1 << 19,  // don't report as hovered when nav focus is on this item
    ImGuiButtonFlags_NoSetKeyOwner          = 1 << 20,  // don't set key/input owner on the initial click (note: mouse buttons are keys! often, the key in question will be ImGuiKey_MouseLeft!)
    ImGuiButtonFlags_NoTestKeyOwner         = 1 << 21,  // don't test key/input owner when polling the key (note: mouse buttons are keys! often, the key in question will be ImGuiKey_MouseLeft!)
    ImGuiButtonFlags_PressedOnMask_         = ImGuiButtonFlags_PressedOnClick | ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnClickReleaseAnywhere | ImGuiButtonFlags_PressedOnRelease | ImGuiButtonFlags_PressedOnDoubleClick | ImGuiButtonFlags_PressedOnDragDropHold,
    ImGuiButtonFlags_PressedOnDefault_      = ImGuiButtonFlags_PressedOnClickRelease,
};

// Extend ImGuiSliderFlags_
enum ImGuiSliderFlagsPrivate_
{
    ImGuiSliderFlags_Vertical               = 1 << 20,  // Should this slider be orientated vertically?
    ImGuiSliderFlags_ReadOnly               = 1 << 21,
};

// Extend ImGuiSelectableFlags_
enum ImGuiSelectableFlagsPrivate_
{
    // NB: need to be in sync with last value of ImGuiSelectableFlags_
    ImGuiSelectableFlags_NoHoldingActiveID      = 1 << 20,
    ImGuiSelectableFlags_SelectOnNav            = 1 << 21,  // (WIP) Auto-select when moved into. This is not exposed in public API as to handle multi-select and modifiers we will need user to explicitly control focus scope. May be replaced with a BeginSelection() API.
    ImGuiSelectableFlags_SelectOnClick          = 1 << 22,  // Override button behavior to react on Click (default is Click+Release)
    ImGuiSelectableFlags_SelectOnRelease        = 1 << 23,  // Override button behavior to react on Release (default is Click+Release)
    ImGuiSelectableFlags_SpanAvailWidth         = 1 << 24,  // Span all avail width even if we declared less for layout purpose. FIXME: We may be able to remove this (added in 6251d379, 2bcafc86 for menus)
    ImGuiSelectableFlags_SetNavIdOnHover        = 1 << 25,  // Set Nav/Focus ID on mouse hover (used by MenuItem)
    ImGuiSelectableFlags_NoPadWithHalfSpacing   = 1 << 26,  // Disable padding each side with ItemSpacing * 0.5f
    ImGuiSelectableFlags_NoSetKeyOwner          = 1 << 27,  // Don't set key/input owner on the initial click (note: mouse buttons are keys! often, the key in question will be ImGuiKey_MouseLeft!)
};

// Extend ImGuiTreeNodeFlags_
enum ImGuiTreeNodeFlagsPrivate_
{
    ImGuiTreeNodeFlags_ClipLabelForTrailingButton = 1 << 20,
};

enum ImGuiSeparatorFlags_
{
    ImGuiSeparatorFlags_None                    = 0,
    ImGuiSeparatorFlags_Horizontal              = 1 << 0,   // Axis default to current layout type, so generally Horizontal unless e.g. in a menu bar
    ImGuiSeparatorFlags_Vertical                = 1 << 1,
    ImGuiSeparatorFlags_SpanAllColumns          = 1 << 2,
};

enum ImGuiTextFlags_
{
    ImGuiTextFlags_None                         = 0,
    ImGuiTextFlags_NoWidthForLargeClippedText   = 1 << 0,
};

enum ImGuiTooltipFlags_
{
    ImGuiTooltipFlags_None                      = 0,
    ImGuiTooltipFlags_OverridePreviousTooltip   = 1 << 0,   // Override will clear/ignore previously submitted tooltip (defaults to append)
};

// FIXME: this is in development, not exposed/functional as a generic feature yet.
// Horizontal/Vertical enums are fixed to 0/1 so they may be used to index ImVec2
enum ImGuiLayoutType_
{
    ImGuiLayoutType_Horizontal = 0,
    ImGuiLayoutType_Vertical = 1
};

// X/Y enums are fixed to 0/1 so they may be used to index ImVec2
enum ImGuiAxis
{
    ImGuiAxis_None = -1,
    ImGuiAxis_X = 0,
    ImGuiAxis_Y = 1
};

enum ImGuiPlotType
{
    ImGuiPlotType_Lines,
    ImGuiPlotType_Histogram,
};

enum ImGuiPopupPositionPolicy
{
    ImGuiPopupPositionPolicy_Default,
    ImGuiPopupPositionPolicy_ComboBox,
    ImGuiPopupPositionPolicy_Tooltip,
};

struct ImGuiDataTypeTempStorage
{
    ImU8        Data[8];        // Can fit any data up to ImGuiDataType_COUNT
};

// Type information associated to one ImGuiDataType. Retrieve with DataTypeGetInfo().
struct ImGuiDataTypeInfo
{
    size_t Size = 0;                // Size in bytes
    const char* Name = nullptr;     // Short descriptive name for the type, for debugging
    const char* PrintFmt = nullptr; // Default printf format for the type
    const char* ScanFmt = nullptr;  // Default scanf format for the type
};

// Extend ImGuiDataType_
enum ImGuiDataTypePrivate_
{
    ImGuiDataType_String = ImGuiDataType_COUNT + 1,
    ImGuiDataType_Pointer,
    ImGuiDataType_ID,
};

// Stacked style modifier, backup of modified data so we can restore it. Data type inferred from the variable.
struct ImGuiStyleMod
{
    ImGuiStyleVar   VarIdx;
    union           { int BackupInt[2]; float BackupFloat[2]; };
    ImGuiStyleMod(ImGuiStyleVar idx, int v)     { VarIdx = idx; BackupInt[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, float v)   { VarIdx = idx; BackupFloat[0] = v; }
    ImGuiStyleMod(ImGuiStyleVar idx, ImVec2 v)  { VarIdx = idx; BackupFloat[0] = v.x; BackupFloat[1] = v.y; }
};

// Stacked storage data for BeginGroup()/EndGroup()
struct ImGuiGroupData
{
    ImGuiID WindowID = 0;
    ImVec2 BackupCursorPos;
    ImVec2 BackupCursorMaxPos;
    ImVec1 BackupIndent;
    ImVec1 BackupGroupOffset;
    ImVec2 BackupCurrLineSize;
    float BackupCurrLineTextBaseOffset = 0.0f;
    ImGuiID BackupActiveIdIsAlive = 0;
    bool BackupActiveIdPreviousFrameIsAlive = false;
    bool BackupHoveredIdIsAlive = false;
    bool EmitItem = false;
};

// Simple column measurement, currently used for MenuItem() only.. This is very short-sighted/throw-away code and NOT a generic helper.
struct ImGuiMenuColumns
{
    ImU32 TotalWidth = 0;
    ImU32 NextTotalWidth = 0;
    ImU16 Spacing = 0;
    ImU16 OffsetIcon = 0;  // Always zero for now
    ImU16 OffsetLabel = 0; // Offsets are locked in Update()
    ImU16 OffsetShortcut = 0;
    ImU16 OffsetMark = 0;
    ImU16 Widths[4] = {0}; // Width of:   Icon, Label, Shortcut, Mark  (accumulators for current frame)

    void Update(float spacing, bool window_reappearing);
    float DeclColumns(float w_icon, float w_label, float w_shortcut, float w_mark);
    void CalcNextTotalWidth(bool update_offsets);
};

// Internal state of the currently focused/edited text input box
// For a given item ID, access with ImGui::GetInputTextState()
struct ImGuiInputTextState
{
    ImGuiContext* Ctx = nullptr;         // parent dear imgui context
    ImGuiID ID = 0;                      // widget id owning the text state
    int CurLenW = 0;
    int CurLenA = 0;                     // we need to maintain our buffer length in both UTF-8 and wchar format. UTF-8 length is valid even if TextA is not.
    ImVector<ImWchar> TextW;             // edit buffer, we need to persist but can't guarantee the persistence of the user-provided buffer. so we copy into own buffer.
    ImVector<char> TextA;                // temporary UTF8 buffer for callbacks and other operations. this is not updated in every code-path! size=capacity.
    ImVector<char> InitialTextA;         // backup of end-user buffer at the time of focus (in UTF-8, unaltered)
    bool TextAIsValid = false;           // temporary UTF8 buffer is not initially valid before we make the widget active (until then we pull the data from user argument)
    int BufCapacityA = 0;                // end-user buffer capacity
    float ScrollX = 0.0f;                // horizontal scrolling/offset
    ImStb::STB_TexteditState Stb;        // state for stb_textedit.h
    float CursorAnim = 0.0f;             // timer for cursor blink, reset on every user action so the cursor reappears immediately
    bool CursorFollow = false;           // set when we want scrolling to follow the current cursor position (not always!)
    bool SelectedAllMouseLock = false;   // after a double-click to select all, we ignore further mouse drags to update selection
    bool Edited = false;                 // edited this frame
    ImGuiInputTextFlags Flags = ImGuiInputTextFlags_None; // copy of InputText() flags. may be used to check if e.g. ImGuiInputTextFlags_Password is set.

    ImGuiInputTextState(ImGuiContext* ctx) : Ctx(ctx) {}

    void ClearText() {
        CurLenW = CurLenA = 0;
        TextW[0] = 0;
        TextA[0] = 0;
        CursorClamp();
    }
    void ClearFreeMemory() {
        TextW.clear();
        TextA.clear();
        InitialTextA.clear();
    }
    int GetUndoAvailCount() const {
        return Stb.undostate.undo_point;
    }
    int GetRedoAvailCount() const {
        return STB_TEXTEDIT_UNDOSTATECOUNT - Stb.undostate.redo_point;
    }
    void OnKeyPressed(int key); // Cannot be inline because we call in code in stb_textedit.h implementation

    // Cursor & Selection
    void CursorAnimReset()  { CursorAnim = -0.30f; }                                   // After a user-input the cursor stays on for a while without blinking
    void CursorClamp() {
        Stb.cursor = ImMin(Stb.cursor, CurLenW);
        Stb.select_start = ImMin(Stb.select_start, CurLenW);
        Stb.select_end = ImMin(Stb.select_end, CurLenW);
    }
    bool HasSelection() const {
        return Stb.select_start != Stb.select_end;
    }
    void ClearSelection() {
        Stb.select_start = Stb.select_end = Stb.cursor;
    }
    int GetCursorPos() const {
        return Stb.cursor;
    }
    int GetSelectionStart() const {
        return Stb.select_start;
    }
    int GetSelectionEnd() const {
        return Stb.select_end;
    }
    void SelectAll() {
        Stb.select_start = 0;
        Stb.cursor = Stb.select_end = CurLenW;
        Stb.has_preferred_x = 0;
    }
};

// Storage for current popup stack
struct ImGuiPopupData
{
    ImGuiID PopupId = 0;                    // Set on OpenPopup()
    ImGuiWindow* Window = nullptr;          // Resolved on BeginPopup() - may stay unresolved if user never calls OpenPopup()
    ImGuiWindow* BackupNavWindow = nullptr; // Set on OpenPopup(), a NavWindow that will be restored on popup close
    int ParentNavLayer = -1;                // Resolved on BeginPopup(). Actually a ImGuiNavLayer type (declared down below), initialized to -1 which is not part of an enum, but serves well-enough as "not any of layers" value
    int OpenFrameCount = -1;                // Set on OpenPopup()
    ImGuiID OpenParentId = 0;               // Set on OpenPopup(), we need this to differentiate multiple menu sets from each others (e.g. inside menu bar vs loose menu items)
    ImVec2 OpenPopupPos;                    // Set on OpenPopup(), preferred popup position (typically == OpenMousePos when using mouse)
    ImVec2 OpenMousePos;                    // Set on OpenPopup(), copy of mouse position at the time of opening popup
};

enum ImGuiNextWindowDataFlags_
{
    ImGuiNextWindowDataFlags_None               = 0,
    ImGuiNextWindowDataFlags_HasPos             = 1 << 0,
    ImGuiNextWindowDataFlags_HasSize            = 1 << 1,
    ImGuiNextWindowDataFlags_HasContentSize     = 1 << 2,
    ImGuiNextWindowDataFlags_HasCollapsed       = 1 << 3,
    ImGuiNextWindowDataFlags_HasSizeConstraint  = 1 << 4,
    ImGuiNextWindowDataFlags_HasFocus           = 1 << 5,
    ImGuiNextWindowDataFlags_HasBgAlpha         = 1 << 6,
    ImGuiNextWindowDataFlags_HasScroll          = 1 << 7,
};

// Storage for SetNexWindow** functions
struct ImGuiNextWindowData
{
    ImGuiNextWindowDataFlags Flags = ImGuiNextWindowDataFlags_None;
    ImGuiCond PosCond = 0;
    ImGuiCond SizeCond = 0;
    ImGuiCond CollapsedCond = 0;
    ImVec2 PosVal;
    ImVec2 PosPivotVal;
    ImVec2 SizeVal;
    ImVec2 ContentSizeVal;
    ImVec2 ScrollVal;
    bool CollapsedVal = false;
    ImRect SizeConstraintRect;
    ImGuiSizeCallback SizeCallback = nullptr;
    void* SizeCallbackUserData = nullptr;
    float BgAlphaVal = 0.0f;       // Override background alpha
    ImVec2 MenuBarOffsetMinVal;    // (Always on) This is not exposed publicly, so we don't clear it and it doesn't have a corresponding flag (could we? for consistency?)

    void ClearFlags() { Flags = ImGuiNextWindowDataFlags_None; }
};

enum ImGuiNextItemDataFlags_
{
    ImGuiNextItemDataFlags_None     = 0,
    ImGuiNextItemDataFlags_HasWidth = 1 << 0,
    ImGuiNextItemDataFlags_HasOpen  = 1 << 1,
};

struct ImGuiNextItemData
{
    ImGuiNextItemDataFlags Flags = ImGuiNextItemDataFlags_None;
    float Width = 0.0f;         // Set by SetNextItemWidth()
    ImGuiID FocusScopeId = 0;   // Set by SetNextItemMultiSelectData() (!= 0 signify value has been set, so it's an alternate version of HasSelectionData, we don't use Flags for this because they are cleared too early. This is mostly used for debugging)
    ImGuiCond OpenCond = 0;
    bool OpenVal = false;       // Set by SetNextItemOpen()

    void ClearFlags() { Flags = ImGuiNextItemDataFlags_None; } // Also cleared manually by ItemAdd()!
};

// Status storage for the last submitted item
struct ImGuiLastItemData
{
    ImGuiID ID = 0;
    ImGuiItemFlags InFlags = ImGuiItemFlags_None;
    ImGuiItemStatusFlags StatusFlags = ImGuiItemStatusFlags_None;
    ImRect Rect;               // Full rectangle
    ImRect NavRect;            // Navigation scoring rectangle (not displayed)
    ImRect DisplayRect;        // Display rectangle (only if ImGuiItemStatusFlags_HasDisplayRect is set)
};

// Data saved for each window pushed into the stack
struct ImGuiWindowStackData
{
    ImGuiWindow* Window = nullptr;
    ImGuiLastItemData ParentLastItemDataBackup;
};

struct ImGuiShrinkWidthItem
{
    int Index = 0;
    float Width = 0.0f;
    float InitialWidth = 0.0f;
};

struct ImGuiPtrOrIndex
{
    void* Ptr; // Either field can be set, not both. e.g. Dock node tab bars are loose while BeginTabBar() ones are in a pool.
    int Index; // Usually index in a main pool.

    ImGuiPtrOrIndex(void* ptr)  { Ptr = ptr; Index = -1; }
    ImGuiPtrOrIndex(int index)  { Ptr = NULL; Index = index; }
};

//-----------------------------------------------------------------------------
// [SECTION] Inputs support
//-----------------------------------------------------------------------------

// [Internal] Key ranges
#define ImGuiKey_LegacyNativeKey_BEGIN  0
#define ImGuiKey_LegacyNativeKey_END    512
#define ImGuiKey_Keyboard_BEGIN         (ImGuiKey_NamedKey_BEGIN)
#define ImGuiKey_Keyboard_END           (ImGuiKey_GamepadStart)
#define ImGuiKey_Gamepad_BEGIN          (ImGuiKey_GamepadStart)
#define ImGuiKey_Gamepad_END            (ImGuiKey_GamepadRStickDown + 1)
#define ImGuiKey_Mouse_BEGIN            (ImGuiKey_MouseLeft)
#define ImGuiKey_Mouse_END              (ImGuiKey_MouseWheelY + 1)
#define ImGuiKey_Aliases_BEGIN          (ImGuiKey_Mouse_BEGIN)
#define ImGuiKey_Aliases_END            (ImGuiKey_Mouse_END)

// [Internal] Named shortcuts for Navigation
#define ImGuiKey_NavKeyboardTweakSlow   ImGuiMod_Ctrl
#define ImGuiKey_NavKeyboardTweakFast   ImGuiMod_Shift
#define ImGuiKey_NavGamepadTweakSlow    ImGuiKey_GamepadL1
#define ImGuiKey_NavGamepadTweakFast    ImGuiKey_GamepadR1
#define ImGuiKey_NavGamepadActivate     ImGuiKey_GamepadFaceDown
#define ImGuiKey_NavGamepadCancel       ImGuiKey_GamepadFaceRight
#define ImGuiKey_NavGamepadMenu         ImGuiKey_GamepadFaceLeft
#define ImGuiKey_NavGamepadInput        ImGuiKey_GamepadFaceUp

enum ImGuiInputEventType
{
    ImGuiInputEventType_None = 0,
    ImGuiInputEventType_MousePos,
    ImGuiInputEventType_MouseWheel,
    ImGuiInputEventType_MouseButton,
    ImGuiInputEventType_Key,
    ImGuiInputEventType_Text,
    ImGuiInputEventType_Focus,
    ImGuiInputEventType_COUNT
};

enum ImGuiInputSource
{
    ImGuiInputSource_None = 0,
    ImGuiInputSource_Mouse,
    ImGuiInputSource_Keyboard,
    ImGuiInputSource_Gamepad,
    ImGuiInputSource_Clipboard,     // Currently only used by InputText()
    ImGuiInputSource_Nav,           // Stored in g.ActiveIdSource only
    ImGuiInputSource_COUNT
};

// FIXME: Structures in the union below need to be declared as anonymous unions appears to be an extension?
// Using ImVec2() would fail on Clang 'union member 'MousePos' has a non-trivial default constructor'
struct ImGuiInputEventMousePos      { float PosX, PosY; };
struct ImGuiInputEventMouseWheel    { float WheelX, WheelY; };
struct ImGuiInputEventMouseButton   { int Button; bool Down; };
struct ImGuiInputEventKey           { ImGuiKey Key; bool Down; float AnalogValue; };
struct ImGuiInputEventText          { unsigned int Char; };
struct ImGuiInputEventAppFocused    { bool Focused; };

struct ImGuiInputEvent
{
    ImGuiInputEventType Type = ImGuiInputEventType_None;
    ImGuiInputSource Source = ImGuiInputSource_None;
    union
    {
        ImGuiInputEventMousePos     MousePos;       // if Type == ImGuiInputEventType_MousePos
        ImGuiInputEventMouseWheel   MouseWheel;     // if Type == ImGuiInputEventType_MouseWheel
        ImGuiInputEventMouseButton  MouseButton;    // if Type == ImGuiInputEventType_MouseButton
        ImGuiInputEventKey          Key;            // if Type == ImGuiInputEventType_Key
        ImGuiInputEventText         Text;           // if Type == ImGuiInputEventType_Text
        ImGuiInputEventAppFocused   AppFocused;     // if Type == ImGuiInputEventType_Focus
    };
};

// Input function taking an 'ImGuiID owner_id' argument defaults to (ImGuiKeyOwner_Any == 0) aka don't test ownership, which matches legacy behavior.
#define ImGuiKeyOwner_Any           ((ImGuiID)0)    // Accept key that have an owner, UNLESS a call to SetKeyOwner() explicitly used ImGuiInputFlags_LockThisFrame or ImGuiInputFlags_LockUntilRelease.
#define ImGuiKeyOwner_None          ((ImGuiID)-1)   // Require key to have no owner.

typedef ImS16 ImGuiKeyRoutingIndex;

// Routing table entry (sizeof() == 16 bytes)
struct ImGuiKeyRoutingData
{
    ImGuiKeyRoutingIndex NextEntryIndex = -1;
    ImU16 Mods = 0;               // Technically we'd only need 4-bits but for simplify we store ImGuiMod_ values which need 16-bits. ImGuiMod_Shortcut is already translated to Ctrl/Super.
    ImU8 RoutingNextScore = 255;   // Lower is better (0: perfect score)
    ImGuiID RoutingCurr = ImGuiKeyOwner_None;
    ImGuiID RoutingNext = ImGuiKeyOwner_None;
};

// Routing table: maintain a desired owner for each possible key-chord (key + mods), and setup owner in NewFrame() when mods are matching.
// Stored in main context (1 instance)
struct ImGuiKeyRoutingTable
{
    ImGuiKeyRoutingIndex            Index[ImGuiKey_NamedKey_COUNT]; // Index of first entry in Entries[]
    ImVector<ImGuiKeyRoutingData>   Entries;
    ImVector<ImGuiKeyRoutingData>   EntriesNext;                    // Double-buffer to avoid reallocation (could use a shared buffer)

    ImGuiKeyRoutingTable()          { Clear(); }
    void Clear()                    { for (int n = 0; n < IM_ARRAYSIZE(Index); n++) Index[n] = -1; Entries.clear(); EntriesNext.clear(); }
};

// This extends ImGuiKeyData but only for named keys (legacy keys don't support the new features)
// Stored in main context (1 per named key). In the future it might be merged into ImGuiKeyData.
struct ImGuiKeyOwnerData
{
    ImGuiID OwnerCurr = ImGuiKeyOwner_None;
    ImGuiID OwnerNext = ImGuiKeyOwner_None;
    bool LockThisFrame = false;      // Reading this key requires explicit owner id (until end of frame). Set by ImGuiInputFlags_LockThisFrame.
    bool LockUntilRelease = false;   // Reading this key requires explicit owner id (until key is released). Set by ImGuiInputFlags_LockUntilRelease. When this is true LockThisFrame is always true as well.
};

// Flags for extended versions of IsKeyPressed(), IsMouseClicked(), Shortcut(), SetKeyOwner(), SetItemKeyOwner()
// Don't mistake with ImGuiInputTextFlags! (for ImGui::InputText() function)
enum ImGuiInputFlags_
{
    // Flags for IsKeyPressed(), IsMouseClicked(), Shortcut()
    ImGuiInputFlags_None                = 0,
    ImGuiInputFlags_Repeat              = 1 << 0,   // Return true on successive repeats. Default for legacy IsKeyPressed(). NOT Default for legacy IsMouseClicked(). MUST BE == 1.
    ImGuiInputFlags_RepeatRateDefault   = 1 << 1,   // Repeat rate: Regular (default)
    ImGuiInputFlags_RepeatRateNavMove   = 1 << 2,   // Repeat rate: Fast
    ImGuiInputFlags_RepeatRateNavTweak  = 1 << 3,   // Repeat rate: Faster
    ImGuiInputFlags_RepeatRateMask_     = ImGuiInputFlags_RepeatRateDefault | ImGuiInputFlags_RepeatRateNavMove | ImGuiInputFlags_RepeatRateNavTweak,

    // Flags for SetItemKeyOwner()
    ImGuiInputFlags_CondHovered         = 1 << 4,   // Only set if item is hovered (default to both)
    ImGuiInputFlags_CondActive          = 1 << 5,   // Only set if item is active (default to both)
    ImGuiInputFlags_CondDefault_        = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,
    ImGuiInputFlags_CondMask_           = ImGuiInputFlags_CondHovered | ImGuiInputFlags_CondActive,

    // Flags for SetKeyOwner(), SetItemKeyOwner()
    ImGuiInputFlags_LockThisFrame       = 1 << 6,   // Access to key data will require EXPLICIT owner ID (ImGuiKeyOwner_Any/0 will NOT accepted for polling). Cleared at end of frame. This is useful to make input-owner-aware code steal keys from non-input-owner-aware code.
    ImGuiInputFlags_LockUntilRelease    = 1 << 7,   // Access to key data will require EXPLICIT owner ID (ImGuiKeyOwner_Any/0 will NOT accepted for polling). Cleared when the key is released or at end of each frame if key is released. This is useful to make input-owner-aware code steal keys from non-input-owner-aware code.

    // Routing policies for Shortcut() + low-level SetShortcutRouting()
    // - The general idea is that several callers register interest in a shortcut, and only one owner gets it.
    // - When a policy (other than _RouteAlways) is set, Shortcut() will register itself with SetShortcutRouting(),
    //   allowing the system to decide where to route the input among other route-aware calls.
    // - Shortcut() uses ImGuiInputFlags_RouteFocused by default: meaning that a simple Shortcut() poll
    //   will register a route and only succeed when parent window is in the focus stack and if no-one
    //   with a higher priority is claiming the shortcut.
    // - Using ImGuiInputFlags_RouteAlways is roughly equivalent to doing e.g. IsKeyPressed(key) + testing mods.
    // - Priorities: GlobalHigh > Focused (when owner is active item) > Global > Focused (when focused window) > GlobalLow.
    // - Can select only 1 policy among all available.
    ImGuiInputFlags_RouteFocused        = 1 << 8,   // (Default) Register focused route: Accept inputs if window is in focus stack. Deep-most focused window takes inputs. ActiveId takes inputs over deep-most focused window.
    ImGuiInputFlags_RouteGlobalLow      = 1 << 9,   // Register route globally (lowest priority: unless a focused window or active item registered the route) -> recommended Global priority.
    ImGuiInputFlags_RouteGlobal         = 1 << 10,  // Register route globally (medium priority: unless an active item registered the route, e.g. CTRL+A registered by InputText).
    ImGuiInputFlags_RouteGlobalHigh     = 1 << 11,  // Register route globally (highest priority: unlikely you need to use that: will interfere with every active items)
    ImGuiInputFlags_RouteMask_          = ImGuiInputFlags_RouteFocused | ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_RouteGlobalLow | ImGuiInputFlags_RouteGlobalHigh, // _Always not part of this!
    ImGuiInputFlags_RouteAlways         = 1 << 12,  // Do not register route, poll keys directly.
    ImGuiInputFlags_RouteUnlessBgFocused= 1 << 13,  // Global routes will not be applied if underlying background/void is focused (== no Dear ImGui windows are focused). Useful for overlay applications.
    ImGuiInputFlags_RouteExtraMask_     = ImGuiInputFlags_RouteAlways | ImGuiInputFlags_RouteUnlessBgFocused,

    // [Internal] Mask of which function support which flags
    ImGuiInputFlags_SupportedByIsKeyPressed     = ImGuiInputFlags_Repeat | ImGuiInputFlags_RepeatRateMask_,
    ImGuiInputFlags_SupportedByShortcut         = ImGuiInputFlags_Repeat | ImGuiInputFlags_RepeatRateMask_ | ImGuiInputFlags_RouteMask_ | ImGuiInputFlags_RouteExtraMask_,
    ImGuiInputFlags_SupportedBySetKeyOwner      = ImGuiInputFlags_LockThisFrame | ImGuiInputFlags_LockUntilRelease,
    ImGuiInputFlags_SupportedBySetItemKeyOwner  = ImGuiInputFlags_SupportedBySetKeyOwner | ImGuiInputFlags_CondMask_,
};

//-----------------------------------------------------------------------------
// [SECTION] Clipper support
//-----------------------------------------------------------------------------



//-----------------------------------------------------------------------------
// [SECTION] Navigation support
//-----------------------------------------------------------------------------

enum ImGuiActivateFlags_
{
    ImGuiActivateFlags_None                 = 0,
    ImGuiActivateFlags_PreferInput          = 1 << 0,       // Favor activation that requires keyboard text input (e.g. for Slider/Drag). Default if keyboard is available.
    ImGuiActivateFlags_PreferTweak          = 1 << 1,       // Favor activation for tweaking with arrows or gamepad (e.g. for Slider/Drag). Default if keyboard is not available.
    ImGuiActivateFlags_TryToPreserveState   = 1 << 2,       // Request widget to preserve state if it can (e.g. InputText will try to preserve cursor/selection)
};

// Early work-in-progress API for ScrollToItem()
enum ImGuiScrollFlags_
{
    ImGuiScrollFlags_None                   = 0,
    ImGuiScrollFlags_KeepVisibleEdgeX       = 1 << 0,       // If item is not visible: scroll as little as possible on X axis to bring item back into view [default for X axis]
    ImGuiScrollFlags_KeepVisibleEdgeY       = 1 << 1,       // If item is not visible: scroll as little as possible on Y axis to bring item back into view [default for Y axis for windows that are already visible]
    ImGuiScrollFlags_KeepVisibleCenterX     = 1 << 2,       // If item is not visible: scroll to make the item centered on X axis [rarely used]
    ImGuiScrollFlags_KeepVisibleCenterY     = 1 << 3,       // If item is not visible: scroll to make the item centered on Y axis
    ImGuiScrollFlags_AlwaysCenterX          = 1 << 4,       // Always center the result item on X axis [rarely used]
    ImGuiScrollFlags_AlwaysCenterY          = 1 << 5,       // Always center the result item on Y axis [default for Y axis for appearing window)
    ImGuiScrollFlags_NoScrollParent         = 1 << 6,       // Disable forwarding scrolling to parent window if required to keep item/rect visible (only scroll window the function was applied to).
    ImGuiScrollFlags_MaskX_                 = ImGuiScrollFlags_KeepVisibleEdgeX | ImGuiScrollFlags_KeepVisibleCenterX | ImGuiScrollFlags_AlwaysCenterX,
    ImGuiScrollFlags_MaskY_                 = ImGuiScrollFlags_KeepVisibleEdgeY | ImGuiScrollFlags_KeepVisibleCenterY | ImGuiScrollFlags_AlwaysCenterY,
};

enum ImGuiNavHighlightFlags_
{
    ImGuiNavHighlightFlags_None             = 0,
    ImGuiNavHighlightFlags_TypeDefault      = 1 << 0,
    ImGuiNavHighlightFlags_TypeThin         = 1 << 1,
    ImGuiNavHighlightFlags_AlwaysDraw       = 1 << 2,       // Draw rectangular highlight if (g.NavId == id) _even_ when using the mouse.
    ImGuiNavHighlightFlags_NoRounding       = 1 << 3,
};

enum ImGuiNavMoveFlags_
{
    ImGuiNavMoveFlags_None                  = 0,
    ImGuiNavMoveFlags_LoopX                 = 1 << 0,   // On failed request, restart from opposite side
    ImGuiNavMoveFlags_LoopY                 = 1 << 1,
    ImGuiNavMoveFlags_WrapX                 = 1 << 2,   // On failed request, request from opposite side one line down (when NavDir==right) or one line up (when NavDir==left)
    ImGuiNavMoveFlags_WrapY                 = 1 << 3,   // This is not super useful but provided for completeness
    ImGuiNavMoveFlags_AllowCurrentNavId     = 1 << 4,   // Allow scoring and considering the current NavId as a move target candidate. This is used when the move source is offset (e.g. pressing PageDown actually needs to send a Up move request, if we are pressing PageDown from the bottom-most item we need to stay in place)
    ImGuiNavMoveFlags_AlsoScoreVisibleSet   = 1 << 5,   // Store alternate result in NavMoveResultLocalVisible that only comprise elements that are already fully visible (used by PageUp/PageDown)
    ImGuiNavMoveFlags_ScrollToEdgeY         = 1 << 6,   // Force scrolling to min/max (used by Home/End) // FIXME-NAV: Aim to remove or reword, probably unnecessary
    ImGuiNavMoveFlags_Forwarded             = 1 << 7,
    ImGuiNavMoveFlags_DebugNoResult         = 1 << 8,   // Dummy scoring for debug purpose, don't apply result
    ImGuiNavMoveFlags_FocusApi              = 1 << 9,
    ImGuiNavMoveFlags_Tabbing               = 1 << 10,  // == Focus + Activate if item is Inputable + DontChangeNavHighlight
    ImGuiNavMoveFlags_Activate              = 1 << 11,
    ImGuiNavMoveFlags_DontSetNavHighlight   = 1 << 12,  // Do not alter the visible state of keyboard vs mouse nav highlight
};

enum ImGuiNavLayer
{
    ImGuiNavLayer_Main  = 0,    // Main scrolling layer
    ImGuiNavLayer_Menu  = 1,    // Menu layer (access with Alt)
    ImGuiNavLayer_COUNT
};

struct ImGuiNavItemData
{
    ImGuiWindow* Window = nullptr;                // Init,Move    // Best candidate window (result->ItemWindow->RootWindowForNav == request->Window)
    ImGuiID ID = 0;                               // Init,Move    // Best candidate item ID
    ImGuiID FocusScopeId = 0;                     // Init,Move    // Best candidate focus scope ID
    ImRect RectRel;                               // Init,Move    // Best candidate bounding box in window relative space
    ImGuiItemFlags InFlags = ImGuiItemFlags_None; // ????,Move    // Best candidate item flags
    float DistBox = FLT_MAX;                      //      Move    // Best candidate box distance to current NavId
    float DistCenter = FLT_MAX;                   //      Move    // Best candidate center distance to current NavId
    float DistAxial = FLT_MAX;                    //      Move    // Best candidate axial distance to current NavId
};

//-----------------------------------------------------------------------------
// [SECTION] Multi-select support
//-----------------------------------------------------------------------------

#ifdef IMGUI_HAS_MULTI_SELECT
// <this is filled in 'range_select' branch>
#endif // #ifdef IMGUI_HAS_MULTI_SELECT

//-----------------------------------------------------------------------------
// [SECTION] Docking support
//-----------------------------------------------------------------------------

#ifdef IMGUI_HAS_DOCK
// <this is filled in 'docking' branch>
#endif // #ifdef IMGUI_HAS_DOCK

//-----------------------------------------------------------------------------
// [SECTION] Viewport support
//-----------------------------------------------------------------------------

// ImGuiViewport Private/Internals fields (cardinal sin: we are using inheritance!)
// Every instance of ImGuiViewport is in fact a ImGuiViewportP.
struct ImGuiViewportP : public ImGuiViewport
{
    int DrawListsLastFrame[2] = { -1, -1 };  // Last frame number the background (0) and foreground (1) draw lists were used
    std::unique_ptr<ImDrawList> DrawLists[2]; // Convenience background (0) and foreground (1) draw lists. We use them to draw software mouser cursor when io.MouseDrawCursor is set and to draw most debug overlays.
    ImDrawData DrawDataP;
    ImDrawDataBuilder DrawDataBuilder;

    ImVec2 WorkOffsetMin;          // Work Area: Offset from Pos to top-left corner of Work Area. Generally (0,0) or (0,+main_menu_bar_height). Work Area is Full Area but without menu-bars/status-bars (so WorkArea always fit inside Pos/Size!)
    ImVec2 WorkOffsetMax;          // Work Area: Offset from Pos+Size to bottom-right corner of Work Area. Generally (0,0) or (0,-status_bar_height).
    ImVec2 BuildWorkOffsetMin;     // Work Area: Offset being built during current frame. Generally >= 0.0f.
    ImVec2 BuildWorkOffsetMax;     // Work Area: Offset being built during current frame. Generally <= 0.0f.

 
    // Calculate work rect pos/size given a set of offset (we have 1 pair of offset for rect locked from last frame data, and 1 pair for currently building rect)
    ImVec2 CalcWorkRectPos(const ImVec2& off_min) const {
        return ImVec2(Pos.x + off_min.x, Pos.y + off_min.y);
    }
    ImVec2 CalcWorkRectSize(const ImVec2& off_min, const ImVec2& off_max) const {
        return ImVec2(ImMax(0.0f, Size.x - off_min.x + off_max.x), ImMax(0.0f, Size.y - off_min.y + off_max.y));
    }
    void UpdateWorkRect() {
        WorkPos = CalcWorkRectPos(WorkOffsetMin);
        WorkSize = CalcWorkRectSize(WorkOffsetMin, WorkOffsetMax);
    } // Update public fields

    // Helpers to retrieve ImRect (we don't need to store BuildWorkRect as every access tend to change it, hence the code asymmetry)
    ImRect GetMainRect() const {
        return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
    ImRect GetWorkRect() const {
        return ImRect(WorkPos.x, WorkPos.y, WorkPos.x + WorkSize.x, WorkPos.y + WorkSize.y);
    }
    ImRect GetBuildWorkRect() const {
        ImVec2 pos = CalcWorkRectPos(BuildWorkOffsetMin);
        ImVec2 size = CalcWorkRectSize(BuildWorkOffsetMin, BuildWorkOffsetMax);
        return ImRect(pos.x, pos.y, pos.x + size.x, pos.y + size.y);
    }
};

//-----------------------------------------------------------------------------
// [SECTION] Settings support
//-----------------------------------------------------------------------------

// Windows data saved in imgui.ini file
struct ImGuiWindowSettings
{
    std::string Name;
    ImGuiID ID = 0;
    ImVec2ih Pos;
    ImVec2ih Size;
    bool Collapsed = false;
    bool WantApply = false;  // Set when loaded from .ini data (to enable merging/loading .ini data into an already running context)
    bool WantDelete = false; // Set to invalidate/delete the settings entry   
};

struct ImGuiSettingsHandler
{
    const char* TypeName = nullptr; // Short description stored in .ini file. Disallowed characters: '[' ']'
    ImGuiID TypeHash = 0;           // == ImHashStr(TypeName)
    void (*ClearAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler) = nullptr;                                // Clear all settings data
    void (*ReadInitFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler) = nullptr;                                // Read: Called before reading (in registration order)
    void* (*ReadOpenFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, const char* name) = nullptr;             // Read: Called when entering into a new ini entry e.g. "[Window][Name]"
    void (*ReadLineFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, void* entry, const char* line) = nullptr; // Read: Called for every line of text within an ini entry
    void (*ApplyAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler) = nullptr;                                // Read: Called after reading (in registration order)
    void (*WriteAllFn)(ImGuiContext* ctx, ImGuiSettingsHandler* handler, std::vector<char>& out_buf) = nullptr;      // Write: Output every entries into 'out_buf'
};

//-----------------------------------------------------------------------------
// [SECTION] Localization support
//-----------------------------------------------------------------------------

// This is experimental and not officially supported, it'll probably fall short of features, if/when it does we may backtrack.
enum ImGuiLocKey : int
{
    ImGuiLocKey_TableSizeOne,
    ImGuiLocKey_TableSizeAllFit,
    ImGuiLocKey_TableSizeAllDefault,
    ImGuiLocKey_TableResetOrder,
    ImGuiLocKey_WindowingMainMenuBar,
    ImGuiLocKey_WindowingPopup,
    ImGuiLocKey_WindowingUntitled,
    ImGuiLocKey_COUNT
};

struct ImGuiLocEntry
{
    ImGuiLocKey Key = ImGuiLocKey_TableSizeOne;
    const char* Text = nullptr;
};


//-----------------------------------------------------------------------------
// [SECTION] Metrics, Debug Tools
//-----------------------------------------------------------------------------

enum ImGuiDebugLogFlags_
{
    // Event types
    ImGuiDebugLogFlags_None             = 0,
    ImGuiDebugLogFlags_EventActiveId    = 1 << 0,
    ImGuiDebugLogFlags_EventFocus       = 1 << 1,
    ImGuiDebugLogFlags_EventPopup       = 1 << 2,
    ImGuiDebugLogFlags_EventNav         = 1 << 3,
    ImGuiDebugLogFlags_EventClipper     = 1 << 4,
    ImGuiDebugLogFlags_EventIO          = 1 << 5,
    ImGuiDebugLogFlags_EventMask_       = ImGuiDebugLogFlags_EventActiveId  | ImGuiDebugLogFlags_EventFocus | ImGuiDebugLogFlags_EventPopup | ImGuiDebugLogFlags_EventNav | ImGuiDebugLogFlags_EventClipper | ImGuiDebugLogFlags_EventIO,
    ImGuiDebugLogFlags_OutputToTTY      = 1 << 10,  // Also send output to TTY
};

struct ImGuiMetricsConfig
{
    bool ShowDebugLog = false;
    bool ShowStackTool = false;
    bool ShowWindowsRects = false;
    bool ShowWindowsBeginOrder = false;
    bool ShowTablesRects = false;
    bool ShowDrawCmdMesh = true;
    bool ShowDrawCmdBoundingBoxes = true;
    bool ShowAtlasTintedWithTextColor = false;
    int ShowWindowsRectsType = -1;
    int ShowTablesRectsType = -1;
};

struct ImGuiStackLevelInfo
{
    ImGuiID ID = 0;
    ImS8 QueryFrameCount = 0;  // >= 1: Query in progress
    bool QuerySuccess = false; // Obtained result from DebugHookIdInfo()
    ImGuiDataType DataType : 8 = ImGuiDataType_S8;
    char Desc[57] {};          // Arbitrarily sized buffer to hold a result (FIXME: could replace Results[] with a chunk stream?) FIXME: Now that we added CTRL+C this should be fixed.
};

// State for Stack tool queries
struct ImGuiStackTool
{
    int LastActiveFrame = 0;
    int StackLevel = 0;                 // -1: query stack and resize Results, >= 0: individual stack level
    ImGuiID QueryId = 0;                    // ID to query details for
    ImVector<ImGuiStackLevelInfo> Results;
    bool CopyToClipboardOnCtrlC = false;
    float CopyToClipboardLastTime = -FLT_MAX;
};

//-----------------------------------------------------------------------------
// [SECTION] Generic context hooks
//-----------------------------------------------------------------------------

typedef void (*ImGuiContextHookCallback)(ImGuiContext* ctx, ImGuiContextHook* hook);
enum ImGuiContextHookType { ImGuiContextHookType_NewFramePre, ImGuiContextHookType_NewFramePost, ImGuiContextHookType_EndFramePre, ImGuiContextHookType_EndFramePost, ImGuiContextHookType_RenderPre, ImGuiContextHookType_RenderPost, ImGuiContextHookType_Shutdown, ImGuiContextHookType_PendingRemoval_ };

struct ImGuiContextHook
{
    ImGuiID HookId = 0;     // A unique ID assigned by AddContextHook()
    ImGuiContextHookType Type = ImGuiContextHookType_NewFramePre;
    ImGuiID Owner = 0;
    ImGuiContextHookCallback Callback = nullptr;
    void* UserData = nullptr;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGuiContext (main Dear ImGui context)
//-----------------------------------------------------------------------------
struct ImGuiContext
{
    bool Initialized = false;
    ImGuiIO IO;
    std::vector<ImGuiInputEvent> InputEventsQueue;   // Input events which will be tricked/written into IO structure.
    std::vector<ImGuiInputEvent> InputEventsTrail;   // Past input events processed in NewFrame(). This is to allow domain-specific application to access e.g mouse/pen trail.
    ImGuiStyle Style;
    ImFont* Font = nullptr;                          // (Shortcut) == FontStack.empty() ? IO.Font : FontStack.back()
    float FontSize = 0.0f;                           // (Shortcut) == FontBaseSize * g.CurrentWindow->FontWindowScale == window->FontSize(). Text height for current window.
    float FontBaseSize = 0.0f;                       // (Shortcut) == IO.FontGlobalScale * Font->Scale * Font->FontSize. Base text height.
    ImDrawListSharedData DrawListSharedData;
    double Time = 0.0;
    int FrameCount = 0;
    int FrameCountEnded = -1;
    int FrameCountRendered = -1;
    bool WithinFrameScope = false;                   // Set by NewFrame(), cleared by EndFrame()
    bool WithinFrameScopeWithImplicitWindow = false; // Set by NewFrame(), cleared by EndFrame() when the implicit debug window has been pushed
    bool WithinEndChild = false;                     // Set within EndChild()

    // Windows state
    std::vector<ImGuiWindow*> Windows;                            // Windows, sorted in display order, back to front
    std::vector<ImGuiWindow*> WindowsFocusOrder;                  // Root windows, sorted in focus order, back to front.
    std::vector<ImGuiWindow*> WindowsTempSortBuffer;              // Temporary buffer used in EndFrame() to reorder windows so parents are kept before their child
    std::vector<ImGuiWindowStackData> CurrentWindowStack;
    int WindowsActiveCount = 0;                 // Number of unique windows submitted by frame
    ImVec2 WindowsHoverPadding;                // Padding around resizable windows for which hovering on counts as hovering the window == ImMax(style.TouchExtraPadding, WINDOWS_HOVER_PADDING)
    ImGuiWindow* CurrentWindow = nullptr;                      // Window being drawn into
    ImGuiWindow* HoveredWindow = nullptr;                      // Window the mouse is hovering. Will typically catch mouse inputs.
    ImGuiWindow* HoveredWindowUnderMovingWindow = nullptr;     // Hovered window ignoring MovingWindow. Only set if MovingWindow is set.
    ImGuiWindow* MovingWindow = nullptr;                       // Track the window we clicked on (in order to preserve focus). The actual window that is moved is generally MovingWindow->RootWindow.
    ImGuiWindow* WheelingWindow = nullptr;                     // Track the window we started mouse-wheeling on. Until a timer elapse or mouse has moved, generally keep scrolling the same window even if during the course of scrolling the mouse ends up hovering a child window.
    ImVec2 WheelingWindowRefMousePos;
    int WheelingWindowStartFrame = -1;           // This may be set one frame before WheelingWindow is != NULL
    float WheelingWindowReleaseTimer = 0.0f;
    ImVec2 WheelingWindowWheelRemainder;
    ImVec2 WheelingAxisAvg;

    // Item/widgets state and tracking information
    ImGuiID DebugHookIdInfo = 0;                    // Will call core hooks: DebugHookIdInfo() from GetID functions, used by Stack Tool [next HoveredId/ActiveId to not pull in an extra cache-line]
    ImGuiID HoveredId = 0;                          // Hovered widget, filled during the frame
    ImGuiID HoveredIdPreviousFrame = 0;
    bool HoveredIdAllowOverlap = false;
    bool HoveredIdDisabled = false;                  // At least one widget passed the rect test, but has been discarded by disabled flag or popup inhibit. May be true even if HoveredId == 0.
    float HoveredIdTimer = 0.0f;                     // Measure contiguous hovering time
    float HoveredIdNotActiveTimer = 0.0f;            // Measure contiguous hovering time where the item has not been active
    ImGuiID ActiveId = 0;                            // Active widget
    ImGuiID ActiveIdIsAlive = 0;                     // Active widget has been seen this frame (we can't use a bool as the ActiveId may change within the frame)
    float ActiveIdTimer = 0.0f;
    bool ActiveIdIsJustActivated = false;            // Set at the time of activation for one frame
    bool ActiveIdAllowOverlap = false;               // Active widget allows another widget to steal active id (generally for overlapping widgets, but not always)
    bool ActiveIdNoClearOnFocusLoss = false;         // Disable losing active id if the active id window gets unfocused.
    bool ActiveIdHasBeenPressedBefore = false;       // Track whether the active id led to a press (this is to allow changing between PressOnClick and PressOnRelease without pressing twice). Used by range_select branch.
    bool ActiveIdHasBeenEditedBefore = false;        // Was the value associated to the widget Edited over the course of the Active state.
    bool ActiveIdHasBeenEditedThisFrame = false;
    ImVec2 ActiveIdClickOffset = {-1, -1};           // Clicked offset from upper-left corner, if applicable (currently only set by ButtonBehavior)
    ImGuiWindow* ActiveIdWindow = nullptr;
    ImGuiInputSource ActiveIdSource = ImGuiInputSource_None; // Activating with mouse or nav (gamepad/keyboard)
    int ActiveIdMouseButton = -1;
    ImGuiID ActiveIdPreviousFrame = 0;
    bool ActiveIdPreviousFrameIsAlive = false;
    bool ActiveIdPreviousFrameHasBeenEditedBefore = false;
    ImGuiWindow* ActiveIdPreviousFrameWindow = nullptr;
    ImGuiID LastActiveId = 0;        // Store the last non-zero ActiveId, useful for animation.
    float LastActiveIdTimer = 0.0f;  // Store the last non-zero ActiveId timer since the beginning of activation, useful for animation.

    // [EXPERIMENTAL] Key/Input Ownership + Shortcut Routing system
    // - The idea is that instead of "eating" a given key, we can link to an owner.
    // - Input query can then read input by specifying ImGuiKeyOwner_Any (== 0), ImGuiKeyOwner_None (== -1) or a custom ID.
    // - Routing is requested ahead of time for a given chord (Key + Mods) and granted in NewFrame().
    ImGuiKeyOwnerData KeysOwnerData[ImGuiKey_NamedKey_COUNT];
    ImGuiKeyRoutingTable KeysRoutingTable;
    ImU32 ActiveIdUsingNavDirMask = 0x00;           // Active widget will want to read those nav move requests (e.g. can activate a button and move away from it)
    bool ActiveIdUsingAllKeyboardKeys = false;      // Active widget will want to read all keyboard keys inputs. (FIXME: This is a shortcut for not taking ownership of 100+ keys but perhaps best to not have the inconsistency)

    // Next window/item data
    ImGuiID CurrentFocusScopeId = 0;                // == g.FocusScopeStack.back()
    ImGuiItemFlags CurrentItemFlags = ImGuiItemFlags_None; // == g.ItemFlagsStack.back()
    ImGuiID DebugLocateId = 0;                      // Storage for DebugLocateItemOnHover() feature: this is read by ItemAdd() so we keep it in a hot/cached location
    ImGuiNextItemData       NextItemData;           // Storage for SetNextItem** functions
    ImGuiLastItemData       LastItemData;           // Storage for last submitted item (setup by ItemAdd)
    ImGuiNextWindowData     NextWindowData;         // Storage for SetNextWindow** functions

    // Shared stacks
    std::vector<ImGuiStyleMod> StyleVarStack;       // Stack for PushStyleVar()/PopStyleVar() - inherited by Begin()
    std::vector<ImFont*> FontStack;                 // Stack for PushFont()/PopFont() - inherited by Begin()
    std::vector<ImGuiID> FocusScopeStack;           // Stack for PushFocusScope()/PopFocusScope() - inherited by BeginChild(), pushed into by Begin()
    std::vector<ImGuiItemFlags> ItemFlagsStack;     // Stack for PushItemFlag()/PopItemFlag() - inherited by Begin()
    std::vector<ImGuiGroupData> GroupStack;         // Stack for BeginGroup()/EndGroup() - not inherited by Begin()
    std::vector<ImGuiPopupData> OpenPopupStack;     // Which popups are open (persistent)
    std::vector<ImGuiPopupData> BeginPopupStack;    // Which level of BeginPopup() we are in (reset every frame)
    int BeginMenuCount = 0;

    // Viewports
    std::vector<std::unique_ptr<ImGuiViewportP>> Viewports; // Active viewports (Size==1 in 'master' branch). Each viewports hold their copy of ImDrawData.

    // Gamepad/keyboard Navigation
    ImGuiWindow* NavWindow = nullptr;               // Focused window for navigation. Could be called 'FocusedWindow'
    ImGuiID NavId = 0;                              // Focused item for navigation
    ImGuiID NavFocusScopeId = 0;                    // Identify a selection scope (selection code often wants to "clear other items" when landing on an item of the selection set)
    ImGuiID NavActivateId = 0;                      // ~~ (g.ActiveId == 0) && (IsKeyPressed(ImGuiKey_Space) || IsKeyPressed(ImGuiKey_NavGamepadActivate)) ? NavId : 0, also set when calling ActivateItem()
    ImGuiID NavActivateDownId = 0;                  // ~~ IsKeyDown(ImGuiKey_Space) || IsKeyDown(ImGuiKey_NavGamepadActivate) ? NavId : 0
    ImGuiID NavActivatePressedId = 0;               // ~~ IsKeyPressed(ImGuiKey_Space) || IsKeyPressed(ImGuiKey_NavGamepadActivate) ? NavId : 0 (no repeat)
    ImGuiID NavActivateInputId = 0;                 // ~~ IsKeyPressed(ImGuiKey_Enter) || IsKeyPressed(ImGuiKey_NavGamepadInput) ? NavId : 0; ImGuiActivateFlags_PreferInput will be set and NavActivateId will be 0.
    ImGuiActivateFlags NavActivateFlags = ImGuiActivateFlags_None;
    ImGuiID NavJustMovedToId = 0;                   // Just navigated to this id (result of a successfully MoveRequest).
    ImGuiID NavJustMovedToFocusScopeId = 0;         // Just navigated to this focus scope id (result of a successfully MoveRequest).
    ImGuiKeyChord NavJustMovedToKeyMods = ImGuiMod_None;
    ImGuiID NavNextActivateId = 0;                  // Set by ActivateItem(), queued until next frame.
    ImGuiActivateFlags NavNextActivateFlags = ImGuiActivateFlags_None;
    ImGuiInputSource NavInputSource = ImGuiInputSource_None; // Keyboard or Gamepad mode? THIS WILL ONLY BE None or NavGamepad or NavKeyboard.
    ImGuiNavLayer NavLayer = ImGuiNavLayer_Main;    // Layer we are navigating on. For now the system is hard-coded for 0=main contents and 1=menu/title bar, may expose layers later.
    bool NavIdIsAlive = false;                      // Nav widget has been seen this frame ~~ NavRectRel is valid
    bool NavMousePosDirty = false;                  // When set we will update mouse position if (io.ConfigFlags & ImGuiConfigFlags_NavEnableSetMousePos) if set (NB: this not enabled by default)
    bool NavDisableHighlight = true;                // When user starts using mouse, we hide gamepad/keyboard highlight (NB: but they are still available, which is why NavDisableHighlight isn't always != NavDisableMouseHover)
    bool NavDisableMouseHover = false;              // When user starts using gamepad/keyboard, we hide mouse hovering highlight until mouse is touched again.

    // Navigation: Init & Move Requests
    bool NavAnyRequest = false;                     // ~~ NavMoveRequest || NavInitRequest this is to perform early out in ItemAdd()
    bool NavInitRequest = false;                    // Init request for appearing window to select first item
    bool NavInitRequestFromMove = false;
    ImGuiID NavInitResultId = 0;                    // Init request result (first item of the window, or one for which SetItemDefaultFocus() was called)
    ImRect NavInitResultRectRel;                    // Init request result rectangle (relative to parent window)
    bool NavMoveSubmitted = false;                  // Move request submitted, will process result on next NewFrame()
    bool NavMoveScoringItems = false;               // Move request submitted, still scoring incoming items
    bool NavMoveForwardToNextFrame = false;
    ImGuiNavMoveFlags NavMoveFlags = ImGuiNavMoveFlags_None;
    ImGuiScrollFlags NavMoveScrollFlags = ImGuiScrollFlags_None;
    ImGuiKeyChord NavMoveKeyMods = ImGuiMod_None;
    ImGuiDir NavMoveDir = ImGuiDir_None;            // Direction of the move request (left/right/up/down)
    ImGuiDir NavMoveDirForDebug = ImGuiDir_None;
    ImGuiDir NavMoveClipDir = ImGuiDir_None;        // FIXME-NAV: Describe the purpose of this better. Might want to rename?
    ImRect NavScoringRect;                          // Rectangle used for scoring, in screen space. Based of window->NavRectRel[], modified for directional navigation scoring.
    ImRect NavScoringNoClipRect;                    // Some nav operations (such as PageUp/PageDown) enforce a region which clipper will attempt to always keep submitted
    int NavTabbingDir = 0;                          // Generally -1 or +1, 0 when tabbing without a nav id
    int NavTabbingCounter = 0;                      // >0 when counting items for tabbing
    ImGuiNavItemData NavMoveResultLocal;            // Best move request candidate within NavWindow
    ImGuiNavItemData NavMoveResultLocalVisible;     // Best move request candidate within NavWindow that are mostly visible (when using ImGuiNavMoveFlags_AlsoScoreVisibleSet flag)
    ImGuiNavItemData NavMoveResultOther;            // Best move request candidate within NavWindow's flattened hierarchy (when using ImGuiWindowFlags_NavFlattened flag)
    ImGuiNavItemData NavTabbingResultFirst;         // First tabbing request candidate within NavWindow and flattened hierarchy

    // Navigation: Windowing (CTRL+TAB for list, or Menu button + keys or directional pads to move/resize)
    ImGuiKeyChord ConfigNavWindowingKeyNext = ImGuiMod_Ctrl | ImGuiKey_Tab;
    ImGuiKeyChord ConfigNavWindowingKeyPrev = ImGuiMod_Ctrl | ImGuiMod_Shift | ImGuiKey_Tab;
    ImGuiWindow* NavWindowingTarget = nullptr;      // Target window when doing CTRL+Tab (or Pad Menu + FocusPrev/Next), this window is temporarily displayed top-most!
    ImGuiWindow* NavWindowingTargetAnim = nullptr;  // Record of last valid NavWindowingTarget until DimBgRatio and NavWindowingHighlightAlpha becomes 0.0f, so the fade-out can stay on it.
    ImGuiWindow* NavWindowingListWindow = nullptr;  // Internal window actually listing the CTRL+Tab contents
    float NavWindowingTimer = 0.0f;
    float NavWindowingHighlightAlpha = 0.0f;
    bool NavWindowingToggleLayer = false;
    ImVec2 NavWindowingAccumDeltaPos;
    ImVec2 NavWindowingAccumDeltaSize;

    // Render
    float DimBgRatio = 0.0f; // 0.0..1.0 animation when fading in a dimming background (for modal window and CTRL+TAB list)
    ImGuiMouseCursor MouseCursor = ImGuiMouseCursor_Arrow;

    // Drag and Drop
    bool DragDropActive = false;
    bool DragDropWithinSource = false;              // Set when within a BeginDragDropXXX/EndDragDropXXX block for a drag source.
    bool DragDropWithinTarget = false;              // Set when within a BeginDragDropXXX/EndDragDropXXX block for a drag target.
    ImGuiDragDropFlags DragDropSourceFlags = ImGuiDragDropFlags_None;
    int DragDropSourceFrameCount = -1;
    int DragDropMouseButton = -1;
    ImGuiPayload DragDropPayload;
    ImRect DragDropTargetRect;                      // Store rectangle of current target candidate (we favor small targets when overlapping)
    ImGuiID DragDropTargetId = 0;
    ImGuiDragDropFlags DragDropAcceptFlags = ImGuiDragDropFlags_None;
    float DragDropAcceptIdCurrRectSurface = 0.0f;   // Target item surface (we resolve overlapping targets by prioritizing the smaller surface)
    ImGuiID DragDropAcceptIdCurr = 0;               // Target item id (set at the time of accepting the payload)
    ImGuiID DragDropAcceptIdPrev = 0;               // Target item id from previous frame (we need to store this to allow for overlapping drag and drop targets)
    int DragDropAcceptFrameCount = -1;              // Last time a target expressed a desire to accept the source
    ImGuiID DragDropHoldJustPressedId = 0;          // Set when holding a payload just made ButtonBehavior() return a press.
    ImVector<unsigned char> DragDropPayloadBufHeap; // We don't expose the ImVector<> directly, ImGuiPayload only holds pointer+size
    unsigned char DragDropPayloadBufLocal[16];      // Local buffer for small payloads

    // Tables
    ImGuiTable* CurrentTable = nullptr;
    std::vector<ImGuiTableTempData> TablesTempData; // Temporary table data (buffers reused/shared across instances, support nesting)
    ImPool<ImGuiTable> Tables;                   // Persistent table data
    ImVector<float> TablesLastTimeActive;        // Last used timestamp of each tables (SOA, for efficient GC)

    // Tab bars
    ImGuiTabBar* CurrentTabBar = nullptr;
    ImPool<ImGuiTabBar> TabBars;
    ImVector<ImGuiPtrOrIndex> CurrentTabBarStack; // pointers or indices into TabBars
    std::vector<ImGuiShrinkWidthItem> ShrinkWidthBuffer;

    // Hover Delay system
    ImGuiID HoverDelayId = 0;
    ImGuiID HoverDelayIdPreviousFrame = 0;
    float HoverDelayTimer = 0.0f;           // Currently used IsItemHovered(), generally inferred from g.HoveredIdTimer but kept uncleared until clear timer elapse.
    float HoverDelayClearTimer = 0.0f;      // Currently used IsItemHovered(): grace time before g.TooltipHoverTimer gets cleared.

    // Widget state
    ImVec2 MouseLastValidPos;
    ImGuiInputTextState InputTextState;
    ImFont InputTextPasswordFont;
    ImGuiID TempInputId = 0;                      // Temporary text input when CTRL+clicking on a slider, etc.
    ImGuiColorEditFlags ColorEditOptions = ImGuiColorEditFlags_DefaultOptions_;                   // Store user options for color edit widgets

    ImGuiID ColorEditCurrentID = 0;               // Set temporarily while inside of the parent-most ColorEdit4/ColorPicker4 (because they call each others).
    ImGuiID ColorEditSavedID = 0;                 // ID we are saving/restoring HS for
    float ColorEditSavedHue = 0.0f;               // Backup of last Hue associated to LastColor, so we can restore Hue in lossy RGB<>HSV round trips
    float ColorEditSavedSat = 0.0f;               // Backup of last Saturation associated to LastColor, so we can restore Saturation in lossy RGB<>HSV round trips
    ImCol ColorEditSavedColor;                // RGB value with alpha set to 0.

    ImColorf ColorPickerRef;                        // Initial/reference color at the time of opening the color picker.
    float SliderGrabClickOffset = 0.0f;
    float SliderCurrentAccum = 0.0f;              // Accumulated slider delta when using navigation controls.
    bool SliderCurrentAccumDirty = false;         // Has the accumulated slider delta changed since last time we tried to apply it?
    bool DragCurrentAccumDirty = false;
    float DragCurrentAccum = 0.0f;                // Accumulator for dragging modification. Always high-precision, not rounded by end-user precision settings
    float DragSpeedDefaultRatio = 1.0f / 100.0f;  // If speed == 0.0f, uses (max-min) * DragSpeedDefaultRatio
    float ScrollbarClickDeltaToGrabCenter = 0.0f; // Distance between mouse and center of grab box, normalized in parent space. Use storage?
    float DisabledAlphaBackup = 0.0f;             // Backup for style.Alpha for BeginDisabled()
    short DisabledStackSize = 0;
    short TooltipOverrideCount = 0;
    ImVector<char> ClipboardHandlerData;           // If no custom clipboard handler is defined
    ImVector<ImGuiID> MenusIdSubmittedThisFrame;   // A list of menu IDs that were rendered at least once

    // Platform support
    char PlatformLocaleDecimalPoint = '.';    // '.' or *localeconv()->decimal_point

    // Settings
    bool SettingsLoaded = false;
    float SettingsDirtyTimer = 0.0f;                    // Save .ini Settings to memory when time reaches zero
    std::vector<char> SettingsIniData;                  // In memory .ini settings
    std::vector<ImGuiSettingsHandler> SettingsHandlers; // List of .ini settings handlers
    std::vector<ImGuiWindowSettings> SettingsWindows; // ImGuiWindow .ini settings entries
    std::vector<ImGuiTableSettings> SettingsTables;   // ImGuiTable .ini settings entries
    ImVector<ImGuiContextHook> Hooks;                   // Hooks for extensions (e.g. test engine)
    ImGuiID HookIdNext = 0;                             // Next available HookId

    // Localization
    const char* LocalizationTable[ImGuiLocKey_COUNT];

    // Debug Tools
    ImGuiDebugLogFlags DebugLogFlags = ImGuiDebugLogFlags_OutputToTTY;
    std::vector<std::string> DebugLog;
    ImU8 DebugLocateFrames = 0;          // For DebugLocateItemOnHover(). This is used together with DebugLocateId which is in a hot/cached spot above.
    bool DebugItemPickerActive = false;  // Item picker is active (started with DebugStartItemPicker())
    ImU8 DebugItemPickerMouseButton = ImGuiMouseButton_Left;
    ImGuiID DebugItemPickerBreakId = 0;  // Will call IM_DEBUG_BREAK() when encountering this ID
    ImGuiMetricsConfig DebugMetricsConfig;
    ImGuiStackTool DebugStackTool;

    ImGuiFPSCounter FpsCounter;

    int WantCaptureMouseNextFrame = -1;     // Explicit capture override via SetNextFrameWantCaptureMouse()/SetNextFrameWantCaptureKeyboard(). Default to -1.
    int WantCaptureKeyboardNextFrame = -1;  // "
    int WantTextInputNextFrame = -1;
    ImVector<char> TempBuffer;              // Temporary text buffer

    ImGuiContext(std::shared_ptr<ImFontAtlas> shared_font_atlas)
        : InputTextState(this)
    {
        IO.Fonts = shared_font_atlas ? shared_font_atlas : std::make_shared<ImFontAtlas>();

        std::fill(std::begin(DragDropPayloadBufLocal), std::end(DragDropPayloadBufLocal), static_cast<unsigned char>(0));
        std::fill(std::begin(LocalizationTable), std::end(LocalizationTable), nullptr);
    }
};

//-----------------------------------------------------------------------------
// [SECTION] ImGuiWindowTempData, ImGuiWindow
//-----------------------------------------------------------------------------

// Transient per-window data, reset at the beginning of the frame. This used to be called ImGuiDrawContext, hence the DC variable name in ImGuiWindow.
// (That's theory, in practice the delimitation between ImGuiWindow and ImGuiWindowTempData is quite tenuous and could be reconsidered..)
// (This doesn't need a constructor because we zero-clear it as part of ImGuiWindow and all frame-temporary data are setup on Begin)
struct ImGuiWindowTempData
{
    // Layout
    ImVec2 CursorPos;              // Current emitting position, in absolute coordinates.
    ImVec2 CursorPosPrevLine;
    ImVec2 CursorStartPos;         // Initial position after Begin(), generally ~ window position + WindowPadding.
    ImVec2 CursorMaxPos;           // Used to implicitly calculate ContentSize at the beginning of next frame, for scrolling range and auto-resize. Always growing during the frame.
    ImVec2 IdealMaxPos;            // Used to implicitly calculate ContentSizeIdeal at the beginning of next frame, for auto-resize only. Always growing during the frame.
    ImVec2 CurrLineSize;
    ImVec2 PrevLineSize;
    float CurrLineTextBaseOffset = 0.0f; // Baseline offset (0.0f by default on a new line, generally == style.FramePadding.y when a framed item has been added).
    float PrevLineTextBaseOffset = 0.0f;
    bool IsSameLine = false;
    ImVec1 Indent;                 // Indentation / start position from left of window (increased by TreePush/TreePop, etc.)
    ImVec1 ColumnsOffset;          // Offset to the current column (if ColumnsCurrent > 0). FIXME: This and the above should be a stack to allow use cases like Tree->Column->Tree. Need revamp columns API.
    ImVec1 GroupOffset;
    ImVec2 CursorStartPosLossyness;// Record the loss of precision of CursorStartPos due to really large scrolling amount. This is used by clipper to compensentate and fix the most common use case of large scroll area.

    // Keyboard/Gamepad navigation
    ImGuiNavLayer NavLayerCurrent;     // Current layer, 0..31 (we currently only use 0..1)
    short NavLayersActiveMask = 0;     // Which layers have been written to (result from previous frame)
    short NavLayersActiveMaskNext = 0; // Which layers have been written to (accumulator for current frame)
    bool NavHideHighlightOneFrame = false;
    bool NavHasScroll = false;         // Set when scrolling can be used (ScrollMax > 0.0f)

    // Miscellaneous
    bool MenuBarAppending = false;             // FIXME: Remove this
    ImVec2 MenuBarOffset;                      // MenuBarOffset.x is sort of equivalent of a per-layer CursorPos.x, saved/restored as we switch to the menu bar. The only situation when MenuBarOffset.y is > 0 if when (SafeAreaPadding.y > FramePadding.y), often used on TVs.
    ImGuiMenuColumns MenuColumns;              // Simplified columns storage for menu items measurement
    int TreeDepth = 0;                         // Current tree depth.
    ImU32 TreeJumpToParentOnPopMask = 0;       // Store a copy of !g.NavIdIsAlive for TreeDepth 0..31.. Could be turned into a ImU64 if necessary.
    std::vector<ImGuiWindow*> ChildWindows;
    ImGuiStorage* StateStorage = nullptr;      // Current persistent per-window storage (store e.g. tree node open/close state)
    int CurrentTableIdx = 0;                   // Current table index (into g.Tables)
    ImGuiLayoutType LayoutType = ImGuiLayoutType_Horizontal;
    ImGuiLayoutType ParentLayoutType = ImGuiLayoutType_Horizontal; // Layout type of parent window at the time of Begin()

    // Local parameters stacks
    // We store the current settings outside of the vectors to increase memory locality (reduce cache misses). The vectors are rarely modified. Also it allows us to not heap allocate for short-lived windows which are not using those settings.
    float ItemWidth = 0.0f;            // Current item width (>0.0: width in pixels, <0.0: align xx pixels to the right of window).
    float TextWrapPos = 0.0f;          // Current text wrap pos.
    ImVector<float> ItemWidthStack;    // Store item widths to restore (attention: .back() is not == ItemWidth)
    ImVector<float> TextWrapPosStack;  // Store text wrap pos to restore (attention: .back() is not == TextWrapPos)
};

// Storage for one window
struct ImGuiWindow
{
    char* Name = nullptr;                      // Window name, owned by the window.
    ImGuiID ID = 0;                            // == ImHashStr(Name)
    ImGuiWindowFlags Flags = ImGuiWindowFlags_None;
    ImGuiViewportP* Viewport = nullptr;        // Always set in Begin(). Inactive windows may have a NULL value here if their viewport was discarded.
    ImVec2 Pos;                                // Position (always rounded-up to nearest pixel)
    ImVec2 Size;                               // Current size (==SizeFull or collapsed title bar size)
    ImVec2 SizeFull;                           // Size when non collapsed
    ImVec2 ContentSize;                        // Size of contents/scrollable client area (calculated from the extents reach of the cursor) from previous frame. Does not include window decoration or window padding.
    ImVec2 ContentSizeIdeal;
    ImVec2 ContentSizeExplicit;                // Size of contents/scrollable client area explicitly request by the user via SetNextWindowContentSize().
    ImVec2 WindowPadding;                      // Window padding at the time of Begin().
    float WindowRounding = 0.0f;               // Window rounding at the time of Begin(). May be clamped lower to avoid rendering artifacts with title bar, menu bar etc.
    float WindowBorderSize = 0.0f;             // Window border size at the time of Begin().
    float DecoOuterSizeX1 = 0.0f;
    float DecoOuterSizeY1 = 0.0f;   // Left/Up offsets. Sum of non-scrolling outer decorations (X1 generally == 0.0f. Y1 generally = TitleBarHeight + MenuBarHeight). Locked during Begin().
    float DecoOuterSizeX2 = 0.0f;
    float DecoOuterSizeY2 = 0.0f;   // Right/Down offsets (X2 generally == ScrollbarSize.x, Y2 == ScrollbarSizes.y).
    float DecoInnerSizeX1 = 0.0f;
    float DecoInnerSizeY1 = 0.0f;   // Applied AFTER/OVER InnerRect. Specialized for Tables as they use specialized form of clipping and frozen rows/columns are inside InnerRect (and not part of regular decoration sizes).
    int NameBufLen = 0;                        // Size of buffer storing Name. May be larger than strlen(Name)!
    ImGuiID MoveId = 0;                        // == window->GetID("#MOVE")
    ImGuiID ChildId = 0;                       // ID of corresponding item in parent window (for navigation to return from child window to parent window)
    ImVec2 Scroll;
    ImVec2 ScrollMax;
    ImVec2 ScrollTarget = {FLT_MAX, FLT_MAX};      // target scroll position. stored as cursor position with scrolling canceled out, so the highest point is always 0.0f. (FLT_MAX for no change)
    ImVec2 ScrollTargetCenterRatio = {0.5f, 0.5f}; // 0.0f = scroll so that target position is at top, 0.5f = scroll so that target position is centered
    ImVec2 ScrollTargetEdgeSnapDist;               // 0.0f = no snapping, >0.0f snapping threshold
    ImVec2 ScrollbarSizes;                         // Size taken by each scrollbars on their smaller axis. Pay attention! ScrollbarSizes.x == width of the vertical scrollbar, ScrollbarSizes.y = height of the horizontal scrollbar.
    bool ScrollbarX = false;
    bool ScrollbarY = false;             // Are scrollbars visible?
    bool Active = false;                             // Set to true on Begin(), unless Collapsed
    bool WasActive = false;
    bool WriteAccessed = false;                      // Set to true when any widget access the current window
    bool Collapsed = false;                          // Set when collapsing window to become only title-bar
    bool WantCollapseToggle = false;
    bool SkipItems = false;                          // Set when items can safely be all clipped (e.g. window not visible or collapsed)
    bool Appearing = false;                          // Set during the frame where the window is appearing (or re-appearing)
    bool Hidden = false;                             // Do not display (== HiddenFrames*** > 0)
    bool IsFallbackWindow = false;                   // Set on the "Debug##Default" window.
    bool IsExplicitChild = false;                    // Set when passed _ChildWindow, left to false by BeginDocked()
    bool HasCloseButton = false;                     // Set when the window has a close button (p_open != NULL)
    signed char ResizeBorderHeld = 0;                   // Current border being held for resize (-1: none, otherwise 0-3)
    short BeginCount = 0;                         // Number of Begin() during the current frame (generally 0 or 1, 1+ if appending via multiple Begin/End pairs)
    short BeginCountPreviousFrame = 0;            // Number of Begin() during the previous frame
    short BeginOrderWithinParent = 0;             // Begin() order within immediate parent window, if we are a child window. Otherwise 0.
    short BeginOrderWithinContext = 0;            // Begin() order within entire imgui context. This is mostly used for debugging submission order related issues.
    short FocusOrder = 0;                         // Order within WindowsFocusOrder[], altered when windows are focused.
    ImGuiID PopupId = 0;                          // ID in the popup stack when this window is used as a popup/menu (because we use generic Name/ID for recycling)
    ImS8 AutoFitFramesX = -1;
    ImS8 AutoFitFramesY = -1;
    ImS8 AutoFitChildAxises = 0;
    bool AutoFitOnlyGrows = false;
    ImGuiDir AutoPosLastDirection = ImGuiDir_None;
    ImS8 HiddenFramesCanSkipItems = 0;           // Hide the window for N frames
    ImS8 HiddenFramesCannotSkipItems = 0;        // Hide the window for N frames while allowing items to be submitted so we can measure their size
    ImS8 HiddenFramesForRenderOnly = 0;          // Hide the window until frame N at Render() time only
    ImS8 DisableInputsFrames = 0;                // Disable window interactions for N frames
    ImGuiCond SetWindowPosAllowFlags : 8 = 0;    // store acceptable condition flags for SetNextWindowPos() use.
    ImGuiCond SetWindowSizeAllowFlags : 8 = 0;   // store acceptable condition flags for SetNextWindowSize() use.
    ImGuiCond SetWindowCollapsedAllowFlags : 8 = 0; // store acceptable condition flags for SetNextWindowCollapsed() use.
    ImVec2 SetWindowPosVal = {FLT_MAX, FLT_MAX};    // store window position when using a non-zero Pivot (position set needs to be processed when we know the window size)
    ImVec2 SetWindowPosPivot = {FLT_MAX, FLT_MAX};  // store window pivot for positioning. ImVec2(0, 0) when positioning from top-left corner; ImVec2(0.5f, 0.5f) for centering; ImVec2(1, 1) for bottom right.

    ImVector<ImGuiID> IDStack;  // ID stack. ID are hashes seeded with the value at the top of the stack. (In theory this should be in the TempData structure)
    ImGuiWindowTempData DC;     // Temporary per-window data, reset at the beginning of the frame. This used to be called ImGuiDrawContext, hence the "DC" variable name.

    // The best way to understand what those rectangles are is to use the 'Metrics->Tools->Show Windows Rectangles' viewer.
    // The main 'OuterRect', omitted as a field, is window->Rect().
    ImRect OuterRectClipped;       // == Window->Rect() just after setup in Begin(). == window->Rect() for root window.
    ImRect InnerRect;              // Inner rectangle (omit title bar, menu bar, scroll bar)
    ImRect InnerClipRect;          // == InnerRect shrunk by WindowPadding*0.5f on each side, clipped within viewport or parent clip rect.
    ImRect WorkRect;               // Initially covers the whole scrolling region. Reduced by containers e.g columns/tables when active. Shrunk by WindowPadding*1.0f on each side. This is meant to replace ContentRegionRect over time (from 1.71+ onward).
    ImRect ParentWorkRect;         // Backup of WorkRect before entering a container such as columns/tables. Used by e.g. SpanAllColumns functions to easily access. Stacked containers are responsible for maintaining this. // FIXME-WORKRECT: Could be a stack?
    ImRect ClipRect;               // Current clipping/scissoring rectangle, evolve as we are using PushClipRect(), etc. == DrawList->clip_rect_stack.back().
    ImRect ContentRegionRect;      // FIXME: This is currently confusing/misleading. It is essentially WorkRect but not handling of scrolling. We currently rely on it as right/bottom aligned sizing operation need some size to rely on.
    ImVec2ih HitTestHoleSize;      // Define an optional rectangular hole where mouse will pass-through the window.
    ImVec2ih HitTestHoleOffset;

    int LastFrameActive = -1;      // Last frame number the window was Active.
    float LastTimeActive = -1.0f;  // Last timestamp the window was Active (using float as we don't need high precision there)
    float ItemWidthDefault = 0.0f;
    ImGuiStorage StateStorage;

    ImDrawList* DrawList = nullptr;                        // == &DrawListInst (for backward compatibility reason with code using imgui_internal.h we keep this a pointer)
    ImDrawList DrawListInst;
    ImGuiWindow* ParentWindow = nullptr;                   // If we are a child _or_ popup _or_ docked window, this is pointing to our parent. Otherwise NULL.
    ImGuiWindow* ParentWindowInBeginStack = nullptr;
    ImGuiWindow* RootWindow = nullptr;                     // Point to ourself or first ancestor that is not a child window. Doesn't cross through popups/dock nodes.
    ImGuiWindow* RootWindowPopupTree = nullptr;            // Point to ourself or first ancestor that is not a child window. Cross through popups parent<>child.
    ImGuiWindow* RootWindowForTitleBarHighlight = nullptr; // Point to ourself or first ancestor which will display TitleBgActive color when this window is active.
    ImGuiWindow* RootWindowForNav = nullptr;               // Point to ourself or first ancestor which doesn't have the NavFlattened flag.

    ImGuiWindow* NavLastChildNavWindow = nullptr;   // When going to the menu bar, we remember the child window we came from. (This could probably be made implicit if we kept g.Windows sorted by last focused including child window.)
    ImGuiID NavLastIds[ImGuiNavLayer_COUNT] = {0};  // Last known NavId for this window, per layer (0/1)
    ImRect NavRectRel[ImGuiNavLayer_COUNT];         // Reference rectangle, in window relative space
    ImGuiID NavRootFocusScopeId = 0;                // Focus Scope ID at the time of Begin()

public:
    ImGuiWindow(ImGuiContext* context, const char* name);
    ~ImGuiWindow();

    ImGuiID GetID(const char* str, const char* str_end = NULL);
    ImGuiID GetID(const void* ptr);
    ImGuiID GetID(int n);
    ImGuiID GetIDFromRectangle(const ImRect& r_abs);

    // We don't use g.FontSize because the window may be != g.CurrentWindow.
    ImRect  Rect() const {
        return ImRect(Pos.x, Pos.y, Pos.x + Size.x, Pos.y + Size.y);
    }
    float CalcFontSize() const {
        ImGuiContext& g = *GImGui;
        return g.FontBaseSize;
    }
    float TitleBarHeight() const {
        ImGuiContext& g = *GImGui;
        return (Flags & ImGuiWindowFlags_NoTitleBar) ? 0.0f : CalcFontSize() + g.Style.FramePadding.y * 2.0f;
    }
    ImRect TitleBarRect() const {
        return ImRect(Pos, ImVec2(Pos.x + SizeFull.x, Pos.y + TitleBarHeight()));
    }
    float MenuBarHeight() const {
        ImGuiContext& g = *GImGui;
        return (Flags & ImGuiWindowFlags_MenuBar) ? DC.MenuBarOffset.y + CalcFontSize() + g.Style.FramePadding.y * 2.0f : 0.0f;
    }
    ImRect MenuBarRect() const {
        float y1 = Pos.y + TitleBarHeight();
        return ImRect(Pos.x, y1, Pos.x + SizeFull.x, y1 + MenuBarHeight());
    }
};

//-----------------------------------------------------------------------------
// [SECTION] Tab bar, Tab item support
//-----------------------------------------------------------------------------

// Extend ImGuiTabBarFlags_
enum ImGuiTabBarFlagsPrivate_
{
    ImGuiTabBarFlags_DockNode                   = 1 << 20,  // Part of a dock node [we don't use this in the master branch but it facilitate branch syncing to keep this around]
    ImGuiTabBarFlags_IsFocused                  = 1 << 21,
    ImGuiTabBarFlags_SaveSettings               = 1 << 22,  // FIXME: Settings are handled by the docking system, this only request the tab bar to mark settings dirty when reordering tabs
};

// Extend ImGuiTabItemFlags_
enum ImGuiTabItemFlagsPrivate_
{
    ImGuiTabItemFlags_SectionMask_              = ImGuiTabItemFlags_Leading | ImGuiTabItemFlags_Trailing,
    ImGuiTabItemFlags_NoCloseButton             = 1 << 20,  // Track whether p_open was set or not (we'll need this info on the next frame to recompute ContentWidth during layout)
    ImGuiTabItemFlags_Button                    = 1 << 21,  // Used by TabItemButton, change the tab item behavior to mimic a button
};

// Storage for one active tab item (sizeof() 40 bytes)
struct ImGuiTabItem
{
    ImGuiID ID = 0;
    ImGuiTabItemFlags Flags = ImGuiTabItemFlags_None;
    int LastFrameVisible = -1;
    int LastFrameSelected = -1;      // This allows us to infer an ordered list of the last activated tabs with little maintenance
    float Offset = 0.0f;             // Position relative to beginning of tab
    float Width = 0.0f;              // Width currently displayed
    float ContentWidth = 0.0f;       // Width of label, stored during BeginTabItem() call
    float RequestedWidth = -1.0f;    // Width optionally requested by caller, -1.0f is unused
    ImS16 BeginOrder = -1;           // BeginTabItem() order, used to re-order tabs after toggling ImGuiTabBarFlags_Reorderable
    ImS16 IndexDuringLayout = -1;    // Index only used during TabBarLayout(). Tabs gets reordered so 'Tabs[n].IndexDuringLayout == n' but may mismatch during additions.
    bool WantClose = false;          // Marked as closed by SetTabItemClosed()
    std::string Name;

    const char* GetName() const { return Name.empty() ? "N/A" : Name.c_str(); }
};

// Storage for a tab bar
struct ImGuiTabBar
{
    std::vector<ImGuiTabItem> Tabs;
    ImGuiTabBarFlags Flags = ImGuiTabBarFlags_None;
    ImGuiID ID = 0;                     // Zero for tab-bars used by docking
    ImGuiID SelectedTabId = 0;          // Selected tab/window
    ImGuiID NextSelectedTabId = 0;      // Next selected tab/window. Will also trigger a scrolling animation
    ImGuiID VisibleTabId = 0;           // Can occasionally be != SelectedTabId (e.g. when previewing contents for CTRL+TAB preview)
    int CurrFrameVisible = -1;
    int PrevFrameVisible = -1;
    ImRect BarRect;
    float CurrTabsContentsHeight = 0.0f;
    float PrevTabsContentsHeight = 0.0f; // Record the height of contents submitted below the tab bar
    float WidthAllTabs = 0.0f;           // Actual width of all tabs (locked during layout)
    float WidthAllTabsIdeal = 0.0f;      // Ideal width if all tabs were visible and not clipped
    float ScrollingAnim = 0.0f;
    float ScrollingTarget = 0.0f;
    float ScrollingTargetDistToVisibility = 0.0f;
    float ScrollingSpeed = 0.0f;
    float ScrollingRectMinX = 0.0f;
    float ScrollingRectMaxX = 0.0f;
    ImGuiID ReorderRequestTabId = 0;
    ImS16 ReorderRequestOffset = 0;
    ImS8 BeginCount = 0;
    bool WantLayout = false;
    bool VisibleTabWasSubmitted = false;
    bool TabsAddedNew = false;           // Set to true when a new tab item or button has been added to the tab bar during last frame
    ImS16 TabsActiveCount = 0;           // Number of tabs submitted this frame.
    ImS16 LastTabItemIdx = -1;           // Index of last BeginTabItem() tab for use by EndTabItem()
    float ItemSpacingY = 0.0f;
    ImVec2 FramePadding;                 // style.FramePadding locked at the time of BeginTabBar()
    ImVec2 BackupCursorPos;
};

//-----------------------------------------------------------------------------
// [SECTION] Table support
//-----------------------------------------------------------------------------

inline constexpr ImCol IM_COL32_DISABLE = ImCol::Black; // Special sentinel code which cannot be used as a regular color.
#define IMGUI_TABLE_MAX_COLUMNS         512                 // May be further lifted

// Our current column maximum is 64 but we may raise that in the future.
typedef ImS16 ImGuiTableColumnIdx;
typedef ImU16 ImGuiTableDrawChannelIdx;

// [Internal] sizeof() ~ 104
// We use the terminology "Enabled" to refer to a column that is not Hidden by user/api.
// We use the terminology "Clipped" to refer to a column that is out of sight because of scrolling/clipping.
// This is in contrast with some user-facing api such as IsItemVisible() / IsRectVisible() which use "Visible" to mean "not clipped".
struct ImGuiTableColumn
{
    ImGuiTableColumnFlags Flags = ImGuiTableColumnFlags_None; // Flags after some patching (not directly same as provided by user). See ImGuiTableColumnFlags_
    float WidthGiven = 0.0f;                     // Final/actual width visible == (MaxX - MinX), locked in TableUpdateLayout(). May be > WidthRequest to honor minimum width, may be < WidthRequest to honor shrinking columns down in tight space.
    float MinX = 0.0f;                           // Absolute positions
    float MaxX = 0.0f;
    float WidthRequest = -1.0f;                  // Master width absolute value when !(Flags & _WidthStretch). When Stretch this is derived every frame from StretchWeight in TableUpdateLayout()
    float WidthAuto = 0.0f;                      // Automatic width
    float StretchWeight = -1.0f;                 // Master width weight when (Flags & _WidthStretch). Often around ~1.0f initially.
    float InitStretchWeightOrWidth = 0.0f;       // Value passed to TableSetupColumn(). For Width it is a content width (_without padding_).
    ImRect ClipRect;                             // Clipping rectangle for the column
    ImGuiID UserID = 0;                          // Optional, value passed to TableSetupColumn()
    float WorkMinX = 0.0f;                       // Contents region min ~(MinX + CellPaddingX + CellSpacingX1) == cursor start position when entering column
    float WorkMaxX = 0.0f;                       // Contents region max ~(MaxX - CellPaddingX - CellSpacingX2)
    float ItemWidth = 0.0f;                      // Current item width for the column, preserved across rows
    float ContentMaxXFrozen = 0.0f;              // Contents maximum position for frozen rows (apart from headers), from which we can infer content width.
    float ContentMaxXUnfrozen = 0.0f;
    float ContentMaxXHeadersUsed = 0.0f;         // Contents maximum position for headers rows (regardless of freezing). TableHeader() automatically softclip itself + report ideal desired size, to avoid creating extraneous draw calls
    float ContentMaxXHeadersIdeal = 0.0f;
    std::string Name;
    ImGuiTableColumnIdx DisplayOrder = -1;                   // Index within Table's IndexToDisplayOrder[] (column may be reordered by users)
    ImGuiTableColumnIdx IndexWithinEnabledSet = -1;          // Index within enabled/visible set (<= IndexToDisplayOrder)
    ImGuiTableColumnIdx PrevEnabledColumn = -1;              // Index of prev enabled/visible column within Columns[], -1 if first enabled/visible column
    ImGuiTableColumnIdx NextEnabledColumn = -1;              // Index of next enabled/visible column within Columns[], -1 if last enabled/visible column
    ImGuiTableColumnIdx SortOrder = -1;                      // Index of this column within sort specs, -1 if not sorting on this column, 0 for single-sort, may be >0 on multi-sort
    ImGuiTableDrawChannelIdx DrawChannelCurrent = (ImU8)-1;  // Index within DrawSplitter.Channels[]
    ImGuiTableDrawChannelIdx DrawChannelFrozen = (ImU8)-1;   // Draw channels for frozen rows (often headers)
    ImGuiTableDrawChannelIdx DrawChannelUnfrozen = (ImU8)-1; // Draw channels for unfrozen rows
    bool IsEnabled;              // IsUserEnabled && (Flags & ImGuiTableColumnFlags_Disabled) == 0
    bool IsUserEnabled;          // Is the column not marked Hidden by the user? (unrelated to being off view, e.g. clipped by scrolling).
    bool IsUserEnabledNextFrame;
    bool IsVisibleX;             // Is actually in view (e.g. overlapping the host window clipping rectangle, not scrolled).
    bool IsVisibleY;
    bool IsRequestOutput;        // Return value for TableSetColumnIndex() / TableNextColumn(): whether we request user to output contents or not.
    bool IsSkipItems;            // Do we want item submissions to this column to be completely ignored (no layout will happen).
    bool IsPreserveWidthAuto;
    ImS8 NavLayerCurrent;                             // ImGuiNavLayer in 1 byte
    ImU8 AutoFitQueue;                                // Queue of 8 values for the next 8 frames to request auto-fit
    ImU8 CannotSkipItemsQueue;                        // Queue of 8 values for the next 8 frames to disable Clipped/SkipItem
    ImU8 SortDirection : 2 = ImGuiSortDirection_None; // ImGuiSortDirection_Ascending or ImGuiSortDirection_Descending
    ImU8 SortDirectionsAvailCount : 2 = 0;            // Number of available sort directions (0 to 3)
    ImU8 SortDirectionsAvailMask : 4 = 0;             // Mask of available sort directions (1-bit each)
    ImU8 SortDirectionsAvailList = 0;                 // Ordered of available sort directions (2-bits each)
};

// Transient cell data stored per row.
// sizeof() ~ 6
struct ImGuiTableCellData
{
    ImCol BgColor = ImCol::BlackTransp; // Actual color
    ImGuiTableColumnIdx Column = 0;     // Column number
};

// Per-instance data that needs preserving across frames (seemingly most others do not need to be preserved aside from debug needs. Does that means they could be moved to ImGuiTableTempData?)
struct ImGuiTableInstanceData
{
    ImGuiID TableInstanceID = 0;
    float LastOuterHeight = 0.0f;    // Outer height from last frame
    float LastFirstRowHeight = 0.0f; // Height of first row from last frame (FIXME: this is used as "header height" and may be reworked)
    float LastFrozenHeight = 0.0f;   // Height of frozen section from last frame
};

// FIXME-TABLE: more transient data could be stored in a stacked ImGuiTableTempData: e.g. SortSpecs, incoming RowData
struct ImGuiTable
{
    ImGuiID ID = 0;
    ImGuiTableFlags Flags = ImGuiTableFlags_None;
    ImGuiTableTempData* TempData = nullptr;             // Transient data while table is active. Point within g.CurrentTableStack[]
    std::vector<ImGuiTableColumn> Columns;                // Point within RawData[]
    std::vector<ImGuiTableColumnIdx> DisplayOrderToIndex; // Point within RawData[]. Store display order of columns (when not reordered, the values are 0...Count-1)
    std::vector<ImGuiTableCellData> RowCellData;          // Point within RawData[]. Store cells background requests for current row.
    std::vector<bool> EnabledMaskByDisplayOrder;        // Column DisplayOrder -> IsEnabled map
    std::vector<bool> EnabledMaskByIndex;               // Column Index -> IsEnabled map (== not hidden by user/api) in a format adequate for iterating column without touching cold data
    std::vector<bool> VisibleMaskByIndex;               // Column Index -> IsVisibleX|IsVisibleY map (== not hidden by user/api && not hidden by scrolling/cliprect)
    ImGuiTableFlags SettingsLoadedFlags = ImGuiTableFlags_None; // Which data were loaded from the .ini file (e.g. when order is not altered we won't save order)
    int SettingsOffset = 0;       // Offset in g.SettingsTables
    int LastFrameActive = -1;
    int ColumnsCount = 0;         // Number of columns declared in BeginTable()
    int CurrentRow = 0;
    int CurrentColumn = 0;
    ImS16 InstanceCurrent = 0;    // Count of BeginTable() calls with same ID in the same frame (generally 0). This is a little bit similar to BeginCount for a window, but multiple table with same ID look are multiple tables, they are just synched.
    ImS16 InstanceInteracted = 0; // Mark which instance (generally 0) of the same ID is being interacted with
    float RowPosY1 = 0.0f;
    float RowPosY2 = 0.0f;
    float RowMinHeight = 0.0f;    // Height submitted to TableNextRow()
    float RowTextBaseline = 0.0f;
    float RowIndentOffsetX = 0.0f;
    ImGuiTableRowFlags RowFlags : 16 = ImGuiTableRowFlags_None; // Current row flags, see ImGuiTableRowFlags_
    ImGuiTableRowFlags LastRowFlags : 16 = ImGuiTableRowFlags_None;
    int RowBgColorCounter = 0;     // Counter for alternating background colors (can be fast-forwarded by e.g clipper), not same as CurrentRow because header rows typically don't increase this.
    ImCol RowBgColor[2] = {ImCol::BlackTransp, ImCol::BlackTransp};  // Background color override for current row.
    ImCol BorderColorStrong = ImCol::BlackTransp;
    ImCol BorderColorLight = ImCol::BlackTransp;
    float BorderX1 = 0.0f;
    float BorderX2 = 0.0f;
    float HostIndentX = 0.0f;
    float MinColumnWidth = 0.0f;
    float OuterPaddingX = 0.0f;
    float CellPaddingX = 0.0f; // Padding from each borders
    float CellPaddingY = 0.0f;
    float CellSpacingX1 = 0.0f; // Spacing between non-bordered cells
    float CellSpacingX2 = 0.0f;
    float InnerWidth = 0.0f;                            // User value passed to BeginTable(), see comments at the top of BeginTable() for details.
    float ColumnsGivenWidth = 0.0f;                     // Sum of current column width
    float ColumnsAutoFitWidth = 0.0f;                   // Sum of ideal column width in order nothing to be clipped, used for auto-fitting and content width submission in outer window
    float ColumnsStretchSumWeights = 0.0f;              // Sum of weight of all enabled stretching columns
    float ResizedColumnNextWidth = 0.0f;
    float ResizeLockMinContentsX2 = 0.0f;               // Lock minimum contents width while resizing down in order to not create feedback loops. But we allow growing the table.
    float RefScale = 0.0f;                              // Reference scale to be able to rescale columns on font/dpi changes.
    ImRect OuterRect;                                   // Note: for non-scrolling table, OuterRect.Max.y is often FLT_MAX until EndTable(), unless a height has been specified in BeginTable().
    ImRect InnerRect;                                   // InnerRect but without decoration. As with OuterRect, for non-scrolling tables, InnerRect.Max.y is
    ImRect WorkRect;
    ImRect InnerClipRect;
    ImRect BgClipRect;                                  // We use this to cpu-clip cell background color fill, evolve during the frame as we cross frozen rows boundaries
    ImRect Bg0ClipRectForDrawCmd;                       // Actual ImDrawCmd clip rect for BG0/1 channel. This tends to be == OuterWindow->ClipRect at BeginTable() because output in BG0/BG1 is cpu-clipped
    ImRect Bg2ClipRectForDrawCmd;                       // Actual ImDrawCmd clip rect for BG2 channel. This tends to be a correct, tight-fit, because output to BG2 are done by widgets relying on regular ClipRect.
    ImRect HostClipRect;                                // This is used to check if we can eventually merge our columns draw calls into the current draw call of the current window.
    ImRect HostBackupInnerClipRect;                     // Backup of InnerWindow->ClipRect during PushTableBackground()/PopTableBackground()
    ImGuiWindow* OuterWindow = nullptr;                 // Parent window for the table
    ImGuiWindow* InnerWindow = nullptr;                 // Window holding the table data (== OuterWindow or a child window)
    ImDrawListSplitter DrawSplitter;
    ImGuiTableInstanceData InstanceDataFirst;
    ImVector<ImGuiTableInstanceData> InstanceDataExtra; // FIXME-OPT: Using a small-vector pattern would be good.
    ImGuiTableColumnSortSpecs SortSpecsSingle;
    ImVector<ImGuiTableColumnSortSpecs> SortSpecsMulti; // FIXME-OPT: Using a small-vector pattern would be good.
    ImGuiTableSortSpecs SortSpecs;                      // Public facing sorts specs, this is what we return in TableGetSortSpecs()
    ImGuiTableColumnIdx SortSpecsCount = 0;
    ImGuiTableColumnIdx ColumnsEnabledCount = 0;        // Number of enabled columns (<= ColumnsCount)
    ImGuiTableColumnIdx ColumnsEnabledFixedCount = 0;   // Number of enabled columns (<= ColumnsCount)
    ImGuiTableColumnIdx DeclColumnsCount = 0;           // Count calls to TableSetupColumn()
    ImGuiTableColumnIdx HoveredColumnBody = 0;          // Index of column whose visible region is being hovered. Important: == ColumnsCount when hovering empty region after the right-most column!
    ImGuiTableColumnIdx HoveredColumnBorder = 0;        // Index of column whose right-border is being hovered (for resizing).
    ImGuiTableColumnIdx AutoFitSingleColumn = 0;        // Index of single column requesting auto-fit.
    ImGuiTableColumnIdx ResizedColumn = 0;              // Index of column being resized. Reset when InstanceCurrent==0.
    ImGuiTableColumnIdx LastResizedColumn = 0;          // Index of column being resized from previous frame.
    ImGuiTableColumnIdx HeldHeaderColumn = 0;           // Index of column header being held.
    ImGuiTableColumnIdx ReorderColumn = 0;              // Index of column being reordered. (not cleared)
    ImGuiTableColumnIdx ReorderColumnDir = 0;           // -1 or +1
    ImGuiTableColumnIdx LeftMostEnabledColumn = 0;      // Index of left-most non-hidden column.
    ImGuiTableColumnIdx RightMostEnabledColumn = 0;     // Index of right-most non-hidden column.
    ImGuiTableColumnIdx LeftMostStretchedColumn = 0;    // Index of left-most stretched column.
    ImGuiTableColumnIdx RightMostStretchedColumn = 0;   // Index of right-most stretched column.
    ImGuiTableColumnIdx ContextPopupColumn = 0;         // Column right-clicked on, of -1 if opening context menu from a neutral/empty spot
    ImGuiTableColumnIdx FreezeRowsRequest = 0;          // Requested frozen rows count
    ImGuiTableColumnIdx FreezeRowsCount = 0;            // Actual frozen row count (== FreezeRowsRequest, or == 0 when no scrolling offset)
    ImGuiTableColumnIdx FreezeColumnsRequest = 0;       // Requested frozen columns count
    ImGuiTableColumnIdx FreezeColumnsCount = 0;         // Actual frozen columns count (== FreezeColumnsRequest, or == 0 when no scrolling offset)
    ImGuiTableColumnIdx RowCellDataCurrent = 0;         // Index of current RowCellData[] entry in current row
    ImGuiTableDrawChannelIdx DummyDrawChannel = 0;      // Redirect non-visible columns here.
    ImGuiTableDrawChannelIdx Bg2DrawChannelCurrent = 0; // For Selectable() and other widgets drawing across columns after the freezing line. Index within DrawSplitter.Channels[]
    ImGuiTableDrawChannelIdx Bg2DrawChannelUnfrozen = 0;
    bool IsLayoutLocked = false;             // Set by TableUpdateLayout() which is called when beginning the first row.
    bool IsInsideRow = false;                // Set when inside TableBeginRow()/TableEndRow().
    bool IsInitializing = false;
    bool IsSortSpecsDirty = false;
    bool IsUsingHeaders = false;             // Set when the first row had the ImGuiTableRowFlags_Headers flag.
    bool IsContextPopupOpen = false;         // Set when default context menu is open (also see: ContextPopupColumn, InstanceInteracted).
    bool IsSettingsRequestLoad = false;
    bool IsSettingsDirty = false;            // Set when table settings have changed and needs to be reported into ImGuiTableSetttings data.
    bool IsDefaultDisplayOrder = false;      // Set when display order is unchanged from default (DisplayOrder contains 0...Count-1)
    bool IsResetAllRequest = false;
    bool IsResetDisplayOrderRequest = false;
    bool IsUnfrozenRows = false;             // Set when we got past the frozen row.
    bool IsDefaultSizingPolicy = false;      // Set if user didn't explicitly set a sizing policy in BeginTable()
    bool HasScrollbarYCurr = false;          // Whether ANY instance of this table had a vertical scrollbar during the current frame.
    bool HasScrollbarYPrev = false;          // Whether ANY instance of this table had a vertical scrollbar during the previous.
    bool HostSkipItems = false;              // Backup of InnerWindow->SkipItem at the end of BeginTable(), because we will overwrite InnerWindow->SkipItem on a per-column basis
};

// Transient data that are only needed between BeginTable() and EndTable(), those buffers are shared (1 per level of stacked table).
// - Accessing those requires chasing an extra pointer so for very frequently used data we leave them in the main table structure.
// - We also leave out of this structure data that tend to be particularly useful for debugging/metrics.
struct ImGuiTableTempData
{
    int TableIndex = 0;                   // Index in g.Tables.Buf[] pool
    float LastTimeActive = -1.0f;         // Last timestamp this structure was used

    ImVec2 UserOuterSize;                 // outer_size.x passed to BeginTable()

    ImRect HostBackupWorkRect;            // Backup of InnerWindow->WorkRect at the end of BeginTable()
    ImRect HostBackupParentWorkRect;      // Backup of InnerWindow->ParentWorkRect at the end of BeginTable()
    ImVec2 HostBackupPrevLineSize;        // Backup of InnerWindow->DC.PrevLineSize at the end of BeginTable()
    ImVec2 HostBackupCurrLineSize;        // Backup of InnerWindow->DC.CurrLineSize at the end of BeginTable()
    ImVec2 HostBackupCursorMaxPos;        // Backup of InnerWindow->DC.CursorMaxPos at the end of BeginTable()
    ImVec1 HostBackupColumnsOffset;       // Backup of OuterWindow->DC.ColumnsOffset at the end of BeginTable()
    float HostBackupItemWidth = 0.0f;     // Backup of OuterWindow->DC.ItemWidth at the end of BeginTable()
    int HostBackupItemWidthStackSize = 0 ;//Backup of OuterWindow->DC.ItemWidthStack.Size at the end of BeginTable()
};

// sizeof() ~ 12
struct ImGuiTableColumnSettings
{
    float WidthOrWeight = 0.0f;
    ImGuiID UserID = 0;
    ImGuiTableColumnIdx Index = -1;
    ImGuiTableColumnIdx DisplayOrder = -1;
    ImGuiTableColumnIdx SortOrder = -1;
    ImU8 SortDirection : 2 = ImGuiSortDirection_None;
    ImU8 IsEnabled : 1 = 1; // "Visible" in ini file
    ImU8 IsStretch : 1 = 0;
};

struct ImGuiTableSettings
{
    ImGuiID ID = 0; // Set to 0 to invalidate/delete the setting
    ImGuiTableFlags SaveFlags = ImGuiTableFlags_None; // Indicate data we want to save using the Resizable/Reorderable/Sortable/Hideable flags (could be using its own flags..)
    float RefScale = 0.0f; // Reference scale to be able to rescale columns on font/dpi changes.
    ImGuiTableColumnIdx ColumnsCount = 0;
    ImGuiTableColumnIdx ColumnsCountMax = 0; // Maximum number of columns this settings instance can store, we can recycle a settings instance with lower number of columns but not higher
    bool WantApply = false; // Set when loaded from .ini data (to enable merging/loading .ini data into an already running context)
    std::vector<ImGuiTableColumnSettings> ColumnSettings;
};

//-----------------------------------------------------------------------------
// [SECTION] ImGui internal API
// No guarantee of forward compatibility here!
//-----------------------------------------------------------------------------

namespace ImGui
{
    // Windows
    // We should always have a CurrentWindow in the stack (there is an implicit "Debug" window)
    // If this ever crash because g.CurrentWindow is NULL it means that either
    // - ImGui::NewFrame() has never been called, which is illegal.
    // - You are calling ImGui functions after ImGui::EndFrame()/ImGui::Render() and before the next ImGui::NewFrame(), which is also illegal.
    inline ImGuiWindow* GetCurrentWindowRead() {
        ImGuiContext& g = *GImGui;
        return g.CurrentWindow;
    }
    inline ImGuiWindow* GetCurrentWindow() {
        ImGuiContext& g = *GImGui;
        g.CurrentWindow->WriteAccessed = true;
        return g.CurrentWindow;
    }
    ImGuiWindow* FindWindowByID(ImGuiID id);
    ImGuiWindow* FindWindowByName(const char* name);
    void UpdateWindowParentAndRootLinks(ImGuiWindow* window, ImGuiWindowFlags flags, ImGuiWindow* parent_window);
    ImVec2 CalcWindowNextAutoFitSize(ImGuiWindow* window);
    bool IsWindowChildOf(ImGuiWindow* window, ImGuiWindow* potential_parent, bool popup_hierarchy);
    bool IsWindowWithinBeginStackOf(ImGuiWindow* window, ImGuiWindow* potential_parent);
    bool IsWindowAbove(ImGuiWindow* potential_above, ImGuiWindow* potential_below);
    bool IsWindowNavFocusable(ImGuiWindow* window);
    void SetWindowPos(ImGuiWindow* window, const ImVec2& pos, ImGuiCond cond = 0);
    void SetWindowSize(ImGuiWindow* window, const ImVec2& size, ImGuiCond cond = 0);
    void SetWindowCollapsed(ImGuiWindow* window, bool collapsed, ImGuiCond cond = 0);
    void SetWindowHitTestHole(ImGuiWindow* window, const ImVec2& pos, const ImVec2& size);

    inline ImRect WindowRectAbsToRel(ImGuiWindow* window, const ImRect& r) {
        ImVec2 off = window->DC.CursorStartPos;
        return ImRect(r.Min.x - off.x, r.Min.y - off.y, r.Max.x - off.x, r.Max.y - off.y);
    }
    inline ImRect WindowRectRelToAbs(ImGuiWindow* window, const ImRect& r) {
        ImVec2 off = window->DC.CursorStartPos;
        return ImRect(r.Min.x + off.x, r.Min.y + off.y, r.Max.x + off.x, r.Max.y + off.y);
    }

    // Windows: Display Order and Focus Order
    void FocusWindow(ImGuiWindow* window);
    void FocusTopMostWindowUnderOne(ImGuiWindow* under_this_window, ImGuiWindow* ignore_window);
    void BringWindowToFocusFront(ImGuiWindow* window);
    void BringWindowToDisplayFront(ImGuiWindow* window);
    void BringWindowToDisplayBack(ImGuiWindow* window);
    void BringWindowToDisplayBehind(ImGuiWindow* window, ImGuiWindow* above_window);
    int FindWindowDisplayIndex(ImGuiWindow* window);
    ImGuiWindow* FindBottomMostVisibleWindowWithinBeginStack(ImGuiWindow* window);

    // Fonts, drawing
    void SetCurrentFont(ImFont* font);
    inline ImFont* GetDefaultFont() { ImGuiContext& g = *GImGui; return g.IO.FontDefault ? g.IO.FontDefault : g.IO.Fonts->Fonts[0]; }
    inline ImDrawList* GetForegroundDrawList(ImGuiWindow* window) { IM_UNUSED(window); return GetForegroundDrawList(); } // This seemingly unnecessary wrapper simplifies compatibility between the 'master' and 'docking' branches.
    ImDrawList* GetBackgroundDrawList(ImGuiViewport* viewport);                     // get background draw list for the given viewport. this draw list will be the first rendering one. Useful to quickly draw shapes/text behind dear imgui contents.
    ImDrawList* GetForegroundDrawList(ImGuiViewport* viewport);                     // get foreground draw list for the given viewport. this draw list will be the last rendered one. Useful to quickly draw shapes/text over dear imgui contents.

    // Init
    void Initialize();
    void Shutdown();    // Since 1.60 this is a _private_ function. You can call DestroyContext() to destroy the context created by CreateContext().

    // NewFrame
    void UpdateInputEvents(bool trickle_fast_inputs);
    void UpdateHoveredWindowAndCaptureFlags();
    void StartMouseMovingWindow(ImGuiWindow* window);
    void UpdateMouseMovingWindowNewFrame();
    void UpdateMouseMovingWindowEndFrame();

    // Generic context hooks
    ImGuiID AddContextHook(ImGuiContext* context, const ImGuiContextHook* hook);
    void RemoveContextHook(ImGuiContext* context, ImGuiID hook_to_remove);
    void CallContextHooks(ImGuiContext* context, ImGuiContextHookType type);

    // Viewports
    void SetWindowViewport(ImGuiWindow* window, ImGuiViewportP* viewport);

    // Settings
    void MarkIniSettingsDirty();
    void MarkIniSettingsDirty(ImGuiWindow* window);
    void ClearIniSettings();
    void AddSettingsHandler(const ImGuiSettingsHandler* handler);
    void RemoveSettingsHandler(const char* type_name);
    ImGuiSettingsHandler* FindSettingsHandler(const char* type_name);

    // Settings - Windows
    ImGuiWindowSettings* CreateNewWindowSettings(std::string_view name);
    ImGuiWindowSettings* FindWindowSettingsByID(ImGuiID id);
    ImGuiWindowSettings* FindWindowSettingsByWindow(ImGuiWindow* window);
    void ClearWindowSettings(const char* name);

    // Localization
    void LocalizeRegisterEntries(const ImGuiLocEntry* entries, int count);
    inline const char* LocalizeGetMsg(ImGuiLocKey key) {
        ImGuiContext& g = *GImGui;
        const char* msg = g.LocalizationTable[key];
        return msg ? msg : "*Missing Text*";
    }

    // Scrolling
    void SetScrollX(ImGuiWindow* window, float scroll_x);
    void SetScrollY(ImGuiWindow* window, float scroll_y);
    void SetScrollFromPosX(ImGuiWindow* window, float local_x, float center_x_ratio);
    void SetScrollFromPosY(ImGuiWindow* window, float local_y, float center_y_ratio);

    // Early work-in-progress API (ScrollToItem() will become public)
    void ScrollToItem(ImGuiScrollFlags flags = 0);
    void ScrollToRect(ImGuiWindow* window, const ImRect& rect, ImGuiScrollFlags flags = 0);
    ImVec2 ScrollToRectEx(ImGuiWindow* window, const ImRect& rect, ImGuiScrollFlags flags = 0);
    inline void ScrollToBringRectIntoView(ImGuiWindow* window, const ImRect& rect) {
        ScrollToRect(window, rect, ImGuiScrollFlags_KeepVisibleEdgeY);
    }

    // Basic Accessors
    inline ImGuiItemStatusFlags GetItemStatusFlags(){ ImGuiContext& g = *GImGui; return g.LastItemData.StatusFlags; }
    inline ImGuiItemFlags GetItemFlags() { ImGuiContext& g = *GImGui; return g.LastItemData.InFlags; }
    inline ImGuiID GetActiveID() { ImGuiContext& g = *GImGui; return g.ActiveId; }
    inline ImGuiID GetFocusID() { ImGuiContext& g = *GImGui; return g.NavId; }
    void SetActiveID(ImGuiID id, ImGuiWindow* window);
    void SetFocusID(ImGuiID id, ImGuiWindow* window);
    void ClearActiveID();
    ImGuiID GetHoveredID();
    void SetHoveredID(ImGuiID id);
    void KeepAliveID(ImGuiID id);
    void MarkItemEdited(ImGuiID id);     // Mark data associated to given item as "edited", used by IsItemDeactivatedAfterEdit() function.
    void PushOverrideID(ImGuiID id);     // Push given value as-is at the top of the ID stack (whereas PushID combines old and new hashes)
    ImGuiID GetIDWithSeed(const char* str_id_begin, const char* str_id_end, ImGuiID seed);
    ImGuiID GetIDWithSeed(int n, ImGuiID seed);

    // Basic Helpers for widget code
    void ItemSize(const ImVec2& size, float text_baseline_y = -1.0f);
    inline void ItemSize(const ImRect& bb, float text_baseline_y = -1.0f) {
        ItemSize(bb.GetSize(), text_baseline_y); // FIXME: This is a misleading API since we expect CursorPos to be bb.Min.
    } 
    bool ItemAdd(const ImRect& bb, ImGuiID id, const ImRect* nav_bb = NULL, ImGuiItemFlags extra_flags = 0);
    bool ItemHoverable(const ImRect& bb, ImGuiID id);
    bool IsClippedEx(const ImRect& bb, ImGuiID id);
    void SetLastItemData(ImGuiID item_id, ImGuiItemFlags in_flags, ImGuiItemStatusFlags status_flags, const ImRect& item_rect);
    ImVec2 CalcItemSize(ImVec2 size, float default_w, float default_h);
    float CalcWrapWidthForPos(const ImVec2& pos, float wrap_pos_x);
    void PushMultiItemsWidths(int components, float width_full);
    bool IsItemToggledSelection(); // Was the last item selection toggled? (after Selectable(), TreeNode() etc. We only returns toggle _event_ in order to handle clipping correctly)
    ImVec2 GetContentRegionMaxAbs();
    void ShrinkWidths(std::span<ImGuiShrinkWidthItem> items, float width_excess);

    // Parameter stacks (shared)
    void PushItemFlag(ImGuiItemFlags option, bool enabled);
    void PopItemFlag();

    // Popups, Modals, Tooltips
    bool BeginChildEx(const char* name, ImGuiID id, const ImVec2& size_arg, bool border, ImGuiWindowFlags flags);
    void OpenPopupEx(ImGuiID id, ImGuiPopupFlags popup_flags = ImGuiPopupFlags_None);
    void ClosePopupToLevel(size_t remaining, bool restore_focus_to_window_under_popup);
    void ClosePopupsOverWindow(ImGuiWindow* ref_window, bool restore_focus_to_window_under_popup);
    void ClosePopupsExceptModals();
    bool IsPopupOpen(ImGuiID id, ImGuiPopupFlags popup_flags);
    bool BeginPopupEx(ImGuiID id, ImGuiWindowFlags extra_flags);
    void BeginTooltipEx(ImGuiTooltipFlags tooltip_flags, ImGuiWindowFlags extra_window_flags);
    ImRect GetPopupAllowedExtentRect(ImGuiWindow* window);
    ImGuiWindow* GetTopMostPopupModal();
    ImGuiWindow* GetTopMostAndVisiblePopupModal();
    ImVec2 FindBestWindowPosForPopup(ImGuiWindow* window);
    ImVec2 FindBestWindowPosForPopupEx(const ImVec2& ref_pos, const ImVec2& size, ImGuiDir* last_dir, const ImRect& r_outer, const ImRect& r_avoid, ImGuiPopupPositionPolicy policy);

    // Menus
    bool BeginViewportSideBar(const char* name, ImGuiViewport* viewport, ImGuiDir dir, float size, ImGuiWindowFlags window_flags);
    bool BeginMenuEx(const char* label, const char* icon, bool enabled = true);
    bool MenuItemEx(const char* label, const char* icon, const char* shortcut = NULL, bool selected = false, bool enabled = true);

    // Combos
    bool BeginComboPopup(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags);

    // Gamepad/Keyboard Navigation
    void NavInitWindow(ImGuiWindow* window, bool force_reinit);
    void NavInitRequestApplyResult();
    bool NavMoveRequestButNoResultYet();
    void NavMoveRequestSubmit(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags);
    void NavMoveRequestForward(ImGuiDir move_dir, ImGuiDir clip_dir, ImGuiNavMoveFlags move_flags, ImGuiScrollFlags scroll_flags);
    void NavMoveRequestResolveWithLastItem(ImGuiNavItemData* result);
    void NavMoveRequestCancel();
    void NavMoveRequestApplyResult();
    void NavMoveRequestTryWrapping(ImGuiWindow* window, ImGuiNavMoveFlags move_flags);
    void ActivateItem(ImGuiID id);   // Remotely activate a button, checkbox, tree node etc. given its unique ID. activation is queued and processed on the next frame when the item is encountered again.
    void SetNavWindow(ImGuiWindow* window);
    void SetNavID(ImGuiID id, ImGuiNavLayer nav_layer, ImGuiID focus_scope_id, const ImRect& rect_rel);

    // Inputs
    // FIXME: Eventually we should aim to move e.g. IsActiveIdUsingKey() into IsKeyXXX functions.
    inline bool IsNamedKey(ImGuiKey key)         { return key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END; }
    inline bool IsNamedKeyOrModKey(ImGuiKey key) { return (key >= ImGuiKey_NamedKey_BEGIN && key < ImGuiKey_NamedKey_END) || key == ImGuiMod_Ctrl || key == ImGuiMod_Shift || key == ImGuiMod_Alt || key == ImGuiMod_Super || key == ImGuiMod_Shortcut; }
    inline bool IsLegacyKey(ImGuiKey key)        { return key >= ImGuiKey_LegacyNativeKey_BEGIN && key < ImGuiKey_LegacyNativeKey_END; }
    inline bool IsKeyboardKey(ImGuiKey key)      { return key >= ImGuiKey_Keyboard_BEGIN && key < ImGuiKey_Keyboard_END; }
    inline bool IsGamepadKey(ImGuiKey key)       { return key >= ImGuiKey_Gamepad_BEGIN && key < ImGuiKey_Gamepad_END; }
    inline bool IsMouseKey(ImGuiKey key)         { return key >= ImGuiKey_Mouse_BEGIN && key < ImGuiKey_Mouse_END; }
    inline bool IsAliasKey(ImGuiKey key)         { return key >= ImGuiKey_Aliases_BEGIN && key < ImGuiKey_Aliases_END; }

    inline ImGuiKeyChord ConvertShortcutMod(ImGuiKeyChord key_chord) {
        ImGuiContext& g = *GImGui;
        IM_ASSERT_PARANOID(key_chord & ImGuiMod_Shortcut);
        return (key_chord & ~ImGuiMod_Shortcut) | (g.IO.ConfigMacOSXBehaviors ? ImGuiMod_Super : ImGuiMod_Ctrl);
    }
    inline ImGuiKey ConvertSingleModFlagToKey(ImGuiKey key) {
        ImGuiContext& g = *GImGui;
        if (key == ImGuiMod_Ctrl) return ImGuiKey_ReservedForModCtrl;
        if (key == ImGuiMod_Shift) return ImGuiKey_ReservedForModShift;
        if (key == ImGuiMod_Alt) return ImGuiKey_ReservedForModAlt;
        if (key == ImGuiMod_Super) return ImGuiKey_ReservedForModSuper;
        if (key == ImGuiMod_Shortcut) return (g.IO.ConfigMacOSXBehaviors ? ImGuiKey_ReservedForModSuper : ImGuiKey_ReservedForModCtrl);
        return key;
    }

    ImGuiKeyData* GetKeyData(ImGuiKey key);
    std::string GetKeyChordName(ImGuiKeyChord key_chord);
    inline ImGuiKey MouseButtonToKey(ImGuiMouseButton button) {
        IM_ASSERT(button >= 0 && button < ImGuiMouseButton_COUNT);
        return (ImGuiKey)(ImGuiKey_MouseLeft + button);
    }
    bool IsMouseDragPastThreshold(ImGuiMouseButton button, float lock_threshold = -1.0f);
    ImVec2 GetKeyMagnitude2d(ImGuiKey key_left, ImGuiKey key_right, ImGuiKey key_up, ImGuiKey key_down);
    float GetNavTweakPressedAmount(ImGuiAxis axis);
    int CalcTypematicRepeatAmount(float t0, float t1, float repeat_delay, float repeat_rate);
    void GetTypematicRepeatRate(ImGuiInputFlags flags, float* repeat_delay, float* repeat_rate);
    void SetActiveIdUsingAllKeyboardKeys();
    inline bool IsActiveIdUsingNavDir(ImGuiDir dir) {
        ImGuiContext& g = *GImGui;
        return (g.ActiveIdUsingNavDirMask & (1 << dir)) != 0;
    }

    // [EXPERIMENTAL] Low-Level: Key/Input Ownership
    // - The idea is that instead of "eating" a given input, we can link to an owner id.
    // - Ownership is most often claimed as a result of reacting to a press/down event (but occasionally may be claimed ahead).
    // - Input queries can then read input by specifying ImGuiKeyOwner_Any (== 0), ImGuiKeyOwner_None (== -1) or a custom ID.
    // - Legacy input queries (without specifying an owner or _Any or _None) are equivalent to using ImGuiKeyOwner_Any (== 0).
    // - Input ownership is automatically released on the frame after a key is released. Therefore:
    //   - for ownership registration happening as a result of a down/press event, the SetKeyOwner() call may be done once (common case).
    //   - for ownership registration happening ahead of a down/press event, the SetKeyOwner() call needs to be made every frame (happens if e.g. claiming ownership on hover).
    // - SetItemKeyOwner() is a shortcut for common simple case. A custom widget will probably want to call SetKeyOwner() multiple times directly based on its interaction state.
    // - This is marked experimental because not all widgets are fully honoring the Set/Test idioms. We will need to move forward step by step.
    //   Please open a GitHub Issue to submit your usage scenario or if there's a use case you need solved.
    ImGuiID GetKeyOwner(ImGuiKey key);
    void SetKeyOwner(ImGuiKey key, ImGuiID owner_id, ImGuiInputFlags flags = 0);
    void SetItemKeyOwner(ImGuiKey key, ImGuiInputFlags flags = 0);           // Set key owner to last item if it is hovered or active. Equivalent to 'if (IsItemHovered() || IsItemActive()) { SetKeyOwner(key, GetItemID());'.
    bool TestKeyOwner(ImGuiKey key, ImGuiID owner_id);                       // Test that key is either not owned, either owned by 'owner_id'
    inline ImGuiKeyOwnerData* GetKeyOwnerData(ImGuiKey key) {
        if (key & ImGuiMod_Mask_)
            key = ConvertSingleModFlagToKey(key);
        IM_ASSERT(IsNamedKey(key));
        return &GImGui->KeysOwnerData[key - ImGuiKey_NamedKey_BEGIN];
    }

    // [EXPERIMENTAL] High-Level: Input Access functions w/ support for Key/Input Ownership
    // - Important: legacy IsKeyPressed(ImGuiKey, bool repeat=true) _DEFAULTS_ to repeat, new IsKeyPressed() requires _EXPLICIT_ ImGuiInputFlags_Repeat flag.
    // - Expected to be later promoted to public API, the prototypes are designed to replace existing ones (since owner_id can default to Any == 0)
    // - Specifying a value for 'ImGuiID owner' will test that EITHER the key is NOT owned (UNLESS locked), EITHER the key is owned by 'owner'.
    //   Legacy functions use ImGuiKeyOwner_Any meaning that they typically ignore ownership, unless a call to SetKeyOwner() explicitly used ImGuiInputFlags_LockThisFrame or ImGuiInputFlags_LockUntilRelease.
    // - Binding generators may want to ignore those for now, or suffix them with Ex() until we decide if this gets moved into public API.
    bool IsKeyDown(ImGuiKey key, ImGuiID owner_id);
    bool IsKeyPressed(ImGuiKey key, ImGuiID owner_id, ImGuiInputFlags flags = 0);    // Important: when transitioning from old to new IsKeyPressed(): old API has "bool repeat = true", so would default to repeat. New API requiress explicit ImGuiInputFlags_Repeat.
    bool IsKeyReleased(ImGuiKey key, ImGuiID owner_id);
    bool IsMouseDown(ImGuiMouseButton button, ImGuiID owner_id);
    bool IsMouseClicked(ImGuiMouseButton button, ImGuiID owner_id, ImGuiInputFlags flags = 0);
    bool IsMouseReleased(ImGuiMouseButton button, ImGuiID owner_id);

    // [EXPERIMENTAL] Shortcut Routing
    // - ImGuiKeyChord = a ImGuiKey optionally OR-red with ImGuiMod_Alt/ImGuiMod_Ctrl/ImGuiMod_Shift/ImGuiMod_Super.
    //     ImGuiKey_C                 (accepted by functions taking ImGuiKey or ImGuiKeyChord)
    //     ImGuiKey_C | ImGuiMod_Ctrl (accepted by functions taking ImGuiKeyChord)
    //   ONLY ImGuiMod_XXX values are legal to 'OR' with an ImGuiKey. You CANNOT 'OR' two ImGuiKey values.
    // - When using one of the routing flags (e.g. ImGuiInputFlags_RouteFocused): routes requested ahead of time given a chord (key + modifiers) and a routing policy.
    // - Routes are resolved during NewFrame(): if keyboard modifiers are matching current ones: SetKeyOwner() is called + route is granted for the frame.
    // - Route is granted to a single owner. When multiple requests are made we have policies to select the winning route.
    // - Multiple read sites may use the same owner id and will all get the granted route.
    // - For routing: when owner_id is 0 we use the current Focus Scope ID as a default owner in order to identify our location.
    bool Shortcut(ImGuiKeyChord key_chord, ImGuiID owner_id = 0, ImGuiInputFlags flags = 0);
    bool SetShortcutRouting(ImGuiKeyChord key_chord, ImGuiID owner_id = 0, ImGuiInputFlags flags = 0);
    bool TestShortcutRouting(ImGuiKeyChord key_chord, ImGuiID owner_id);
    ImGuiKeyRoutingData* GetShortcutRoutingData(ImGuiKeyChord key_chord);

    // [EXPERIMENTAL] Focus Scope
    // This is generally used to identify a unique input location (for e.g. a selection set)
    // There is one per window (automatically set in Begin), but:
    // - Selection patterns generally need to react (e.g. clear a selection) when landing on one item of the set.
    //   So in order to identify a set multiple lists in same window may each need a focus scope.
    //   If you imagine an hypothetical BeginSelectionGroup()/EndSelectionGroup() api, it would likely call PushFocusScope()/EndFocusScope()
    // - Shortcut routing also use focus scope as a default location identifier if an owner is not provided.
    // We don't use the ID Stack for this as it is common to want them separate.
    void PushFocusScope(ImGuiID id);
    void PopFocusScope();
    inline ImGuiID GetCurrentFocusScope() {
        // Focus scope we are outputting into, set by PushFocusScope()
        ImGuiContext& g = *GImGui;
        return g.CurrentFocusScopeId;
    }

    // Drag and Drop
    bool IsDragDropActive();
    bool BeginDragDropTargetCustom(const ImRect& bb, ImGuiID id);
    void ClearDragDrop();
    bool IsDragDropPayloadBeingAccepted();
    void RenderDragDropTargetRect(const ImRect& bb);

    // Internal Columns API (this is not exposed because we will encourage transitioning to the Tables API)
    void SetWindowClipRectBeforeSetChannel(ImGuiWindow* window, const ImRect& clip_rect);

    // Tables: Candidates for public API
    void TableOpenContextMenu(int column_n = -1);
    void TableSetColumnWidth(int column_n, float width);
    void TableSetColumnSortDirection(int column_n, ImGuiSortDirection sort_direction, bool append_to_sort_specs);
    int  TableGetHoveredColumn(); // May use (TableGetColumnFlags() & ImGuiTableColumnFlags_IsHovered) instead. Return hovered column. return -1 when table is not hovered. return columns_count if the unused space at the right of visible columns is hovered.
    float TableGetHeaderRowHeight();
    void TablePushBackgroundChannel();
    void TablePopBackgroundChannel();

    // Tables: Internals
    inline    ImGuiTable*   GetCurrentTable() { ImGuiContext& g = *GImGui; return g.CurrentTable; }
    ImGuiTable*   TableFindByID(ImGuiID id);
    bool          BeginTableEx(const char* name, ImGuiID id, int columns_count, ImGuiTableFlags flags = 0, const ImVec2& outer_size = ImVec2(0, 0), float inner_width = 0.0f);
    void          TableBeginApplyRequests(ImGuiTable* table);
    void          TableSetupDrawChannels(ImGuiTable* table);
    void          TableUpdateLayout(ImGuiTable* table);
    void          TableUpdateBorders(ImGuiTable* table);
    void          TableUpdateColumnsWeightFromWidth(ImGuiTable* table);
    void          TableDrawBorders(ImGuiTable* table);
    void          TableDrawContextMenu(ImGuiTable* table);
    bool          TableBeginContextMenuPopup(ImGuiTable* table);
    void          TableMergeDrawChannels(ImGuiTable* table);
    inline ImGuiTableInstanceData* TableGetInstanceData(ImGuiTable* table, int instance_no) { if (instance_no == 0) return &table->InstanceDataFirst; return &table->InstanceDataExtra[instance_no - 1]; }
    inline ImGuiID TableGetInstanceID(ImGuiTable* table, int instance_no)   { return TableGetInstanceData(table, instance_no)->TableInstanceID; }
    void          TableSortSpecsSanitize(ImGuiTable* table);
    void          TableSortSpecsBuild(ImGuiTable* table);
    ImGuiSortDirection TableGetColumnNextSortDirection(ImGuiTableColumn* column);
    void          TableFixColumnSortDirection(ImGuiTable* table, ImGuiTableColumn* column);
    float         TableGetColumnWidthAuto(ImGuiTable* table, ImGuiTableColumn* column);
    void          TableBeginRow(ImGuiTable* table);
    void          TableEndRow(ImGuiTable* table);
    void          TableBeginCell(ImGuiTable* table, int column_n);
    void          TableEndCell(ImGuiTable* table);
    ImRect        TableGetCellBgRect(const ImGuiTable* table, int column_n);
    const char*   TableGetColumnName(const ImGuiTable* table, int column_n);
    ImGuiID       TableGetColumnResizeID(ImGuiTable* table, int column_n, int instance_no = 0);
    float         TableGetMaxColumnWidth(const ImGuiTable* table, int column_n);
    void          TableSetColumnWidthAutoSingle(ImGuiTable* table, int column_n);
    void          TableSetColumnWidthAutoAll(ImGuiTable* table);
    void          TableRemove(ImGuiTable* table);

    // Tables: Settings
    void                  TableLoadSettings(ImGuiTable* table);
    void                  TableSaveSettings(ImGuiTable* table);
    void                  TableResetSettings(ImGuiTable* table);
    ImGuiTableSettings*   TableGetBoundSettings(ImGuiTable* table);
    void                  TableSettingsAddSettingsHandler();
    ImGuiTableSettings*   TableSettingsCreate(ImGuiID id, int columns_count);
    int TableSettingsFindByID(ImGuiID id);

    // Tab Bars
    inline ImGuiTabBar* GetCurrentTabBar() {
        ImGuiContext& g = *GImGui;
        return g.CurrentTabBar;
    }
    bool BeginTabBarEx(ImGuiTabBar* tab_bar, const ImRect& bb, ImGuiTabBarFlags flags);

    ImGuiTabItem* TabBarGetCurrentTab(ImGuiTabBar* tab_bar);
    void TabBarRemoveTab(ImGuiTabBar* tab_bar, ImGuiID tab_id);
    void TabBarCloseTab(ImGuiTabBar* tab_bar, ImGuiTabItem& tab);
    void TabBarQueueReorder(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, int offset);
    void TabBarQueueReorderFromMousePos(ImGuiTabBar* tab_bar, ImGuiTabItem* tab, ImVec2 mouse_pos);
    bool TabBarProcessReorder(ImGuiTabBar* tab_bar);
    bool TabItemEx(ImGuiTabBar* tab_bar, const char* label, bool* p_open, ImGuiTabItemFlags flags, ImGuiWindow* docked_window);
    ImVec2 TabItemCalcSize(const char* label, bool has_close_button_or_unsaved_marker);
    ImVec2 TabItemCalcSize(ImGuiWindow* window);
    void TabItemBackground(ImDrawList* draw_list, const ImRect& bb, ImGuiTabItemFlags flags, ImCol col);
    void TabItemLabelAndCloseButton(ImDrawList* draw_list, const ImRect& bb, ImGuiTabItemFlags flags, ImVec2 frame_padding, const char* label, ImGuiID tab_id, ImGuiID close_button_id, bool is_contents_visible, bool* out_just_closed, bool* out_text_clipped);

    // Render helpers
    // AVOID USING OUTSIDE OF IMGUI.CPP! NOT FOR PUBLIC CONSUMPTION. THOSE FUNCTIONS ARE A MESS. THEIR SIGNATURE AND BEHAVIOR WILL CHANGE, THEY NEED TO BE REFACTORED INTO SOMETHING DECENT.
    // NB: All position are in absolute pixels coordinates (we are never using window coordinates internally)
    void RenderText(ImVec2 pos, const char* text, const char* text_end = NULL, bool hide_text_after_hash = true);
    void RenderTextWrapped(ImVec2 pos, const char* text, const char* text_end, float wrap_width);
    void RenderTextClipped(const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
    void RenderTextClippedEx(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, const char* text, const char* text_end, const ImVec2* text_size_if_known, const ImVec2& align = ImVec2(0, 0), const ImRect* clip_rect = NULL);
    void RenderTextEllipsis(ImDrawList* draw_list, const ImVec2& pos_min, const ImVec2& pos_max, float clip_max_x, float ellipsis_max_x, const char* text, const char* text_end, const ImVec2* text_size_if_known);
    void RenderFrame(ImVec2 p_min, ImVec2 p_max, ImCol fill_col, bool border = true, float rounding = 0.0f);
    void RenderFrameBorder(ImVec2 p_min, ImVec2 p_max, float rounding = 0.0f);
    void RenderColorRectWithAlphaCheckerboard(ImDrawList* draw_list, ImVec2 p_min, ImVec2 p_max, ImCol fill_col, float grid_step, ImVec2 grid_off, float rounding = 0.0f, ImDrawFlags flags = 0);
    void RenderNavHighlight(const ImRect& bb, ImGuiID id, ImGuiNavHighlightFlags flags = ImGuiNavHighlightFlags_TypeDefault); // Navigation highlight
    const char* FindRenderedTextEnd(const char* text, const char* text_end = NULL); // Find the optional ## from which we stop displaying text.
    void RenderMouseCursor(ImVec2 pos, float scale, ImGuiMouseCursor mouse_cursor, ImCol col_fill, ImCol col_border, ImCol col_shadow);

    // Render helpers (those functions don't access any ImGui state!)
    void RenderArrow(ImDrawList* draw_list, ImVec2 pos, ImCol col, ImGuiDir dir, float scale = 1.0f);
    void RenderBullet(ImDrawList* draw_list, ImVec2 pos, ImCol col);
    void RenderCheckMark(ImDrawList* draw_list, ImVec2 pos, ImCol col, float sz);
    void RenderArrowPointingAt(ImDrawList* draw_list, ImVec2 pos, ImVec2 half_sz, ImGuiDir direction, ImCol col);
    void RenderRectFilledRangeH(ImDrawList* draw_list, const ImRect& rect, ImCol col, float x_start_norm, float x_end_norm, float rounding);
    void RenderRectFilledWithHole(ImDrawList* draw_list, const ImRect& outer, const ImRect& inner, ImCol col, float rounding);

    // Widgets
    void TextEx(const char* text, const char* text_end = NULL, ImGuiTextFlags flags = 0);
    bool ButtonEx(const char* label, const ImVec2& size_arg = ImVec2(0, 0), ImGuiButtonFlags flags = 0);
    bool ArrowButtonEx(const char* str_id, ImGuiDir dir, ImVec2 size_arg, ImGuiButtonFlags flags = 0);
    bool ImageButtonEx(ImGuiID id, ImTextureID texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImColorf& bg_col, const ImColorf& tint_col, ImGuiButtonFlags flags = 0);
    void SeparatorEx(ImGuiSeparatorFlags flags);
    void SeparatorTextEx(ImGuiID id, const char* label, const char* label_end, float extra_width);
    bool CheckboxFlags(const char* label, ImS64* flags, ImS64 flags_value);
    bool CheckboxFlags(const char* label, ImU64* flags, ImU64 flags_value);

    // Widgets: Window Decorations
    bool    CloseButton(ImGuiID id, const ImVec2& pos);
    bool    CollapseButton(ImGuiID id, const ImVec2& pos);
    void    Scrollbar(ImGuiAxis axis);
    bool    ScrollbarEx(const ImRect& bb, ImGuiID id, ImGuiAxis axis, ImS64* p_scroll_v, ImS64 avail_v, ImS64 contents_v, ImDrawFlags flags);
    ImRect  GetWindowScrollbarRect(ImGuiWindow* window, ImGuiAxis axis);
    ImGuiID GetWindowScrollbarID(ImGuiWindow* window, ImGuiAxis axis);
    ImGuiID GetWindowResizeCornerID(ImGuiWindow* window, int n); // 0..3: corners
    ImGuiID GetWindowResizeBorderID(ImGuiWindow* window, ImGuiDir dir);

    // Widgets low-level behaviors
    bool ButtonBehavior(const ImRect& bb, ImGuiID id, bool* out_hovered, bool* out_held, ImGuiButtonFlags flags = 0);
    bool DragBehavior(ImGuiID id, ImGuiDataType data_type, void* p_v, float v_speed, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags);
    bool SliderBehavior(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, void* p_v, const void* p_min, const void* p_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);
    bool SplitterBehavior(const ImRect& bb, ImGuiID id, ImGuiAxis axis, float* size1, float* size2, float min_size1, float min_size2, float hover_extend = 0.0f, float hover_visibility_delay = 0.0f, ImCol bg_col = ImCol::BlackTransp);
    bool TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end = NULL);
    void TreePushOverrideID(ImGuiID id);
    void TreeNodeSetOpen(ImGuiID id, bool open);
    bool TreeNodeUpdateNextOpen(ImGuiID id, ImGuiTreeNodeFlags flags);   // Return open state. Consume previous SetNextItemOpen() data, if any. May return true when logging.

    // Template functions are instantiated in imgui_widgets.cpp for a finite number of types.
    // To use them externally (for custom widget) you may need an "extern template" statement in your code in order to link to existing instances and silence Clang warnings (see #2036).
    // e.g. " extern template IMGUI_API float RoundScalarWithFormatT<float, float>(const char* format, ImGuiDataType data_type, float v); "
    template<typename T, typename SIGNED_T, typename FLOAT_T>
    float ScaleRatioFromValueT(ImGuiDataType data_type, T v, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);

    template<typename T, typename SIGNED_T, typename FLOAT_T>
    T ScaleValueFromRatioT(ImGuiDataType data_type, float t, T v_min, T v_max, bool is_logarithmic, float logarithmic_zero_epsilon, float zero_deadzone_size);

    template<typename T, typename SIGNED_T, typename FLOAT_T>
    bool DragBehaviorT(ImGuiDataType data_type, T* v, float v_speed, T v_min, T v_max, const char* format, ImGuiSliderFlags flags);

    template<typename T, typename SIGNED_T, typename FLOAT_T>
    bool SliderBehaviorT(const ImRect& bb, ImGuiID id, ImGuiDataType data_type, T* v, T v_min, T v_max, const char* format, ImGuiSliderFlags flags, ImRect* out_grab_bb);

    template<typename T>
    T RoundScalarWithFormatT(const char* format, ImGuiDataType data_type, T v);

    template<typename T>
    bool CheckboxFlagsT(const char* label, T* flags, T flags_value);

    // Data type helpers
    const ImGuiDataTypeInfo*  DataTypeGetInfo(ImGuiDataType data_type);
    int  DataTypeFormatString(char* buf, int buf_size, ImGuiDataType data_type, const void* p_data, const char* format);
    void DataTypeApplyOp(ImGuiDataType data_type, int op, void* output, const void* arg_1, const void* arg_2);
    bool DataTypeApplyFromText(const char* buf, ImGuiDataType data_type, void* p_data, const char* format);
    int  DataTypeCompare(ImGuiDataType data_type, const void* arg_1, const void* arg_2);
    bool DataTypeClamp(ImGuiDataType data_type, void* p_data, const void* p_min, const void* p_max);

    // InputText
    bool InputTextEx(const char* label, const char* hint, char* buf, int buf_size, const ImVec2& size_arg, ImGuiInputTextFlags flags, ImGuiInputTextCallback callback = NULL, void* user_data = NULL);
    bool TempInputText(const ImRect& bb, ImGuiID id, const char* label, char* buf, int buf_size, ImGuiInputTextFlags flags);
    bool TempInputScalar(const ImRect& bb, ImGuiID id, const char* label, ImGuiDataType data_type, void* p_data, const char* format, const void* p_clamp_min = NULL, const void* p_clamp_max = NULL);
    inline bool TempInputIsActive(ImGuiID id) {
        ImGuiContext& g = *GImGui;
        return (g.ActiveId == id && g.TempInputId == id);
    }
    inline ImGuiInputTextState* GetInputTextState(ImGuiID id) {
        // Get input text state if active
        ImGuiContext& g = *GImGui;
        return (id != 0 && g.InputTextState.ID == id) ? &g.InputTextState : NULL;
    }

    // Color
    void ColorTooltip(const char* text, const float* col, ImGuiColorEditFlags flags);
    void ColorEditOptionsPopup(const float* col, ImGuiColorEditFlags flags);
    void ColorPickerOptionsPopup(const float* ref_col, ImGuiColorEditFlags flags);

    // Plot
    int PlotEx(ImGuiPlotType plot_type, const char* label, float (*values_getter)(void* data, int idx), void* data, int values_count, int values_offset, const char* overlay_text, float scale_min, float scale_max, const ImVec2& size_arg);

    // Shade functions (write over already created vertices)
    void ShadeVertsLinearColorGradientKeepAlpha(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, ImVec2 gradient_p0, ImVec2 gradient_p1, ImCol col0, ImCol col1);
    void ShadeVertsLinearUV(ImDrawList* draw_list, int vert_start_idx, int vert_end_idx, const ImVec2& a, const ImVec2& b, const ImVec2& uv_a, const ImVec2& uv_b, bool clamp);

    // Debug Log
    void DebugLog(const char* fmt, ...) IM_FMTARGS(1);
    void DebugLogV(const char* fmt, va_list args) IM_FMTLIST(1);

    // Debug Tools
    void DebugLocateItem(ImGuiID target_id);                     // Call sparingly: only 1 at the same time!
    void DebugLocateItemOnHover(ImGuiID target_id);              // Only call on reaction to a mouse Hover: because only 1 at the same time!
    void DebugLocateItemResolveWithLastItem();
    inline void DebugDrawItemRect(ImCol col = ImCol(255,0,0,255)) {
        ImGuiContext& g = *GImGui;
        ImGuiWindow* window = g.CurrentWindow;
        GetForegroundDrawList(window)->AddRect(g.LastItemData.Rect.Min, g.LastItemData.Rect.Max, col);
    }
    inline void DebugStartItemPicker() { ImGuiContext& g = *GImGui; g.DebugItemPickerActive = true; }
    void ShowFontAtlas(ImFontAtlas* atlas);
    void DebugHookIdInfo(ImGuiID id, ImGuiDataType data_type, const void* data_id, const void* data_id_end);
    void DebugNodeDrawList(ImGuiWindow* window, const ImDrawList* draw_list, const char* label);
    void DebugNodeDrawCmdShowMeshAndBoundingBox(ImDrawList* out_draw_list, const ImDrawList* draw_list, const ImDrawCmd* draw_cmd, bool show_mesh, bool show_aabb);
    void DebugNodeFont(ImFont* font);
    void DebugNodeFontGlyph(ImFont* font, const ImFontGlyph* glyph);
    void DebugNodeStorage(ImGuiStorage* storage, const char* label);
    void DebugNodeTabBar(ImGuiTabBar* tab_bar, const char* label);
    void DebugNodeTable(ImGuiTable* table);
    void DebugNodeTableSettings(ImGuiTableSettings* settings);
    void DebugNodeInputTextState(ImGuiInputTextState* state);
    void DebugNodeWindow(ImGuiWindow* window, const char* label);
    void DebugNodeWindowSettings(const ImGuiWindowSettings& settings);
    void DebugNodeWindowsList(std::vector<ImGuiWindow*>& windows, const char* label);
    void DebugNodeWindowsListByBeginStackParent(std::span<ImGuiWindow*> windows, ImGuiWindow* parent_in_begin_stack);
    void DebugNodeViewport(ImGuiViewportP* viewport);
    void DebugRenderKeyboardPreview(ImDrawList* draw_list);
    void DebugRenderViewportThumbnail(ImDrawList* draw_list, ImGuiViewportP* viewport, const ImRect& bb);

} // namespace ImGui


//-----------------------------------------------------------------------------
// [SECTION] ImFontAtlas internal API
//-----------------------------------------------------------------------------

// This structure is likely to evolve as we add support for incremental atlas updates
struct ImFontBuilderIO
{
    bool    (*FontBuilder_Build)(ImFontAtlas* atlas);
};

// Helper for font builder
#ifdef IMGUI_ENABLE_STB_TRUETYPE
const ImFontBuilderIO* ImFontAtlasGetBuilderForStbTruetype();
#endif
void ImFontAtlasBuildInit(ImFontAtlas* atlas);
void ImFontAtlasBuildSetupFont(ImFontAtlas* atlas, ImFont* font, ImFontConfig* font_config, float ascent, float descent);
void ImFontAtlasBuildPackCustomRects(ImFontAtlas* atlas, void* stbrp_context_opaque);
void ImFontAtlasBuildFinish(ImFontAtlas* atlas);
void ImFontAtlasBuildRender8bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned char in_marker_pixel_value);
void ImFontAtlasBuildRender32bppRectFromString(ImFontAtlas* atlas, int x, int y, int w, int h, const char* in_str, char in_marker_char, unsigned int in_marker_pixel_value);
void ImFontAtlasBuildMultiplyCalcLookupTable(unsigned char out_table[256], float in_multiply_factor);
void ImFontAtlasBuildMultiplyRectAlpha8(const unsigned char table[256], unsigned char* pixels, int x, int y, int w, int h, int stride);

//-----------------------------------------------------------------------------
// [SECTION] Test Engine specific hooks (imgui_test_engine)
//-----------------------------------------------------------------------------

#define IMGUI_TEST_ENGINE_ITEM_INFO(_ID,_LABEL,_FLAGS)      ((void)g)

//-----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#endif

#ifdef _MSC_VER
#pragma warning (pop)
#endif

#endif // #ifndef IMGUI_DISABLE
