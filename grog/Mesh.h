#pragma once

#include "grog.h"

namespace grog
{
  class Mesh
  {
    public:
      const int32_t* vertexBuffer {nullptr};
      uint32_t vertexCount {0};
      const uint32_t* faces {nullptr};
      const Gamebuino_Meta::ColorIndex*  colors {nullptr};
      uint32_t faceCount {0};
  };
}
