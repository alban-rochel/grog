#pragma once


#include <cstdint>
#include "Matrix.h"

namespace grog
{
    constexpr unsigned int screenWidth = 80;
    constexpr unsigned int screenHeight = 64;
    using bufferType = uint16_t;//unsigned char;

    using vec4f = float[4];
    using mat44f = float[4*4];
    using vec4i = int[4];
    using mat44i = int[4*4];

    using trianglef = vec4f[3];
    using trianglei = vec4i[3];

    using coord = int32_t;
    using vertex = coord[3];

    struct Triangle// in normalized cube
    {
        int32_t p1x{0};
        int32_t p1y{0};
        int32_t p2x{0};
        int32_t p2y{0};
        int32_t p3x{0};
        int32_t p3y{0};
        int32_t z{0}; // used for ordering only
        uint8_t color{0};
        Triangle* next {nullptr};
    } ;

    constexpr const int& min2(const int &t1, const int &t2) noexcept
    {
        return (t1 < t2 ? t1 : t2);
    }

    constexpr const int& max2(const int &t1, const int &t2) noexcept
    {
        return (t1 > t2 ? t1 : t2);
    }

    constexpr const int& min3(const int& t1, const int &t2, const int &t3) noexcept
    {
        return (t1 < t2 ? min2(t1, t3) : min2(t2, t3));
    }

    constexpr const int& max3(const int &t1, const int &t2, const int &t3) noexcept
    {
        return (t1 > t2 ? max2(t1, t3) : max2(t2, t3));
    }

    constexpr int orient2d(const int& p1x, const int& p1y,
                           const int& p2x, const int& p2y,
                           const int& p3x, const int& p3y) noexcept
    {
        return (p2x-p1x)*(p3y-p1y) - (p2y-p1y)*(p3x-p1x);
    }

/*    void rasterizeTriangle( const int* __restrict v0,
                            const int* __restrict v1,
                            const int* __restrict v2,
                            const bufferType& color,
                            bufferType* __restrict frameBuffer) noexcept;*/
    void rasterizeTriangle( const Triangle& triangle,
                            bufferType* __restrict frameBuffer) noexcept;

    // Accumulate
//    void computeGeometry( const coord* __restrict vertexBuffer,
//                          const uint32_t* __restrict faces,
//                          const uint8_t* __restrict colors);

    void normalize(float* io) noexcept;

    void crossProd(float* left,
                   float* right,
                   float* out,
                   bool normalize) noexcept;
}
