#pragma once

#ifdef __linux
#include "Gamebuino-Meta-stuff.h"
#else
#include <Gamebuino-Meta.h>
#endif
#include <cstdint>
#include "Matrix.h"

#define FIXED_POINT_OFFSET 10
typedef int32_t Fixed;

namespace grog
{
    GROG_INLINE int32_t floatToFixed(float val) noexcept
    {
      return (int32_t)(val * 1024.f + 0.5f);
    }

    constexpr unsigned int screenWidth = 80;
    constexpr unsigned int screenHeight = 64;
    using bufferType = uint8_t;

    struct Triangle// in normalized cube
    {
        int32_t p1x{0};
        int32_t p1y{0};
        int32_t p2x{0};
        int32_t p2y{0};
        int32_t p3x{0};
        int32_t p3y{0};
        int32_t z{0}; // used for ordering only
        Gamebuino_Meta::ColorIndex color{Gamebuino_Meta::ColorIndex::black};
        Triangle* next {nullptr};
    } ;

    GROG_INLINE const int& min2(const int &t1, const int &t2) noexcept
    {
        return (t1 < t2 ? t1 : t2);
    }

    GROG_INLINE const int& max2(const int &t1, const int &t2) noexcept
    {
        return (t1 > t2 ? t1 : t2);
    }

    GROG_INLINE const int& min3(const int& t1, const int &t2, const int &t3) noexcept
    {
        return (t1 < t2 ? min2(t1, t3) : min2(t2, t3));
    }

    GROG_INLINE const int& max3(const int &t1, const int &t2, const int &t3) noexcept
    {
        return (t1 > t2 ? max2(t1, t3) : max2(t2, t3));
    }

    GROG_INLINE int orient2d(const int& p1x, const int& p1y,
                           const int& p2x, const int& p2y,
                           const int& p3x, const int& p3y) noexcept
    {
        return (p2x-p1x)*(p3y-p1y) - (p2y-p1y)*(p3x-p1x);
    }

    void rasterizeTriangle( const Triangle& triangle,
                            bufferType* __restrict frameBuffer) noexcept;

    void normalize(int32_t* io) noexcept;

    void crossProd(int32_t* left,
                   int32_t* right,
                   int32_t* out,
                   bool normalize) noexcept;
}

