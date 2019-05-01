#pragma once

#ifdef __linux
#include "Gamebuino-Meta-stuff.h"
#else
#include <Gamebuino-Meta.h>
#endif
#include <cstdint>
#include "Matrix.h"

#define GROG_INLINE inline __attribute__((always_inline))
#define GROG_LIKELY(x)       __builtin_expect((x),1)
#define GROG_UNLIKELY(x)     __builtin_expect((x),0)

namespace grog
{
    GROG_INLINE int32_t floatToFixed(float val) noexcept
    {
      return (int32_t)(val * 1024.f + 0.5f);
    }

    constexpr unsigned int screenWidth = 80;
    constexpr unsigned int screenHeight = 64;
    using bufferType = uint8_t;

}

