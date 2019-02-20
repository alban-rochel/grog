#pragma once

#include "grog.h"

namespace grog
{
  class Mesh
  {
    public:
      const coord* vertexBuffer {nullptr};
      uint32_t vertexCount {0};
      const uint32_t* faces {nullptr};
      const uint8_t*  colors {nullptr};
      uint32_t faceCount {0};
  };
}

