#pragma once

#include <stdint.h>

// Primitive types
using s8 = int8_t;
using u8 = uint8_t;
using s16 = int16_t;
using u16 = uint16_t;
using s32 = int32_t;
using u32 = uint32_t;
using s64 = int64_t;
using u64 = uint64_t;

using f32 = float;
using f64 = double;

struct Guid {
    u32 Data1;
    u16 Data2;
    u16 Data3;
    u8 Data4[8];
};

struct Sha1 {
    u32 Data1;
    u32 Data2;
    u32 Data3;
    u32 Data4;
    u32 Data5;
};

// Debug only function attribute
#ifdef _DEBUG
    #define DEBUG_ONLY
#else
    #define DEBUG_ONLY [[maybe_unused]]
#endif

// ALWAYS_INLINE and NOINLINE function attributes
#ifdef _MSC_VER
    #define ALWAYS_INLINE __forceinline
    #define NOINLINE __declspec(noinline)

#elif defined(__GNUC__) || defined(__clang__)
    #define ALWAYS_INLINE inline __attribute__((always_inline))
    #define NOINLINE __attribute__((noinline))

#endif

// ALIGNED function attribute
#ifdef _MSC_VER
    #define ALIGNED(x) __declspec(align(x))

#elif defined(__GNUC__) || defined(__clang__)
    #define ALIGNED(x) __attribute__((aligned(x)))

#endif

#define UNREACHABLE __assume(false);
