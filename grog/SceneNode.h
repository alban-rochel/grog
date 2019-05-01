#pragma once

#include "grog.h"
#include "Mesh.h"
#include "Matrix.h"

namespace grog
{
  class SceneNode
  {
    public:
      SceneNode() noexcept;

      ~SceneNode() noexcept;

      uint32_t renderPass {0x01};

      Mesh mesh;
      TransformMatrix transform;

      SceneNode** children {nullptr};
      uint32_t childCount {0};
  };
}
