#pragma once

#include <cstdint>

#ifdef __linux
#include "Gamebuino-Meta-stuff.h"
#else
#include <Gamebuino-Meta.h>
#endif

#define GROG_INLINE inline __attribute__((always_inline))
#define GROG_LIKELY(x)       __builtin_expect((x),1)
#define GROG_UNLIKELY(x)     __builtin_expect((x),0)

namespace grog
{
  /**
   * @brief The display width, half the full Meta resolution.
   */
  constexpr uint32_t DisplayWidth = 80;
  /**
   * @brief The display height, half the full Meta resolution.
   */
  constexpr uint32_t DisplayHeight = 64;
}

