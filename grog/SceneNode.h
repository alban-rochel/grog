#pragma once

#include "grog.h"
#include "Mesh.h"
#include "Matrix.h"

namespace grog
{
  /**
   * @brief The SceneNode describes a node in the hierarchy of meshes.
   * Each SceneNode contains a (possibly empty) mesh, and a transformation relative to its parent node.
   */
  class SceneNode
  {
    public:

      /**
       * @brief Constructor.
       */
      SceneNode() noexcept = default;

      /**
       * @brief Destructor.
       * Deletes the SceneNodes in m_children.
       */
      ~SceneNode() noexcept;

      uint32_t m_renderPass {0x01}; ///< Which pass this node should be rasterized on.

      Mesh m_mesh {}; ///< The node mesh.
      TransformMatrix m_transform {}; ///< The transformation relative to the parent.

      SceneNode** m_children {nullptr}; ///< The children nodes.
      uint32_t m_childCount {0}; ///< The number of children nodes.
  };
}
