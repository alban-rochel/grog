#pragma once

#include "SceneNode.h"

namespace demo
{
  /**
   * @brief The Road class is a scene node containing  the full description of a road segment.
   * It contains pointers to the meshes, but also the hierarchy of meshes (trees on the road side etc.).
   */
  class Road: public grog::SceneNode
  {
    public:
      /**
       * @brief Constructor.
       * @throws std::bad_alloc if allocation fails.
       */
      Road();
  };
}
