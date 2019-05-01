#pragma once

#include "grog.h"

namespace grog
{
  class Triangle// in normalized cube
  {
    public:
      int32_t p1x{0};
      int32_t p1y{0};
      int32_t p2x{0};
      int32_t p2y{0};
      int32_t p3x{0};
      int32_t p3y{0};
      int32_t z{0}; // used for ordering only
      Gamebuino_Meta::ColorIndex color{Gamebuino_Meta::ColorIndex::black};
      Triangle* next {nullptr};

      void rasterize(uint8_t* __restrict frameBuffer) const noexcept;
  } ;

}
