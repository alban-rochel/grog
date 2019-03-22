#pragma once

#include "Mesh.h"
#include "Matrix.h"

namespace grog
{
  class SceneNode
  {
    public:
      SceneNode() noexcept;

      ~SceneNode() noexcept;

      Mesh mesh;
      TransformMatrix transform;

      SceneNode** children {nullptr};
      uint32_t childCount {0};
  };
}
