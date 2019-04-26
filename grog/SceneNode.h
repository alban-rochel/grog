#pragma once

#include "Mesh.h"
#include "Matrix.h"

namespace grog
{
  /**
   * @brief The SceneNode class
   * The distance between \f$(x_1,y_1)\f$ and \f$(x_2,y_2)\f$ is
  \f$\sqrt{(x_2-x_1)^2+(y_2-y_1)^2}\f$.
   */
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
