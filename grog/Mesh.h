#pragma once

#include "grog.h"

namespace grog
{
  /**
   * @brief The Mesh contains the info describing a 3D object, as a mesh of triangle faces.
   */
  class Mesh
  {
    public:
      /**
       * The vertex buffer, containing a series of triplets of signed Q21.10 values, storing the vertex coordinates.
       */
      const int32_t* m_vertexBuffer {nullptr};
      uint32_t m_vertexCount {0}; ///< The number of vertices, i.e. one third of the size of m_vertexBuffer.

      /**
       * The face buffer, containing a series of triplets of 0-based indices for the vertices describing faces.
       * @warning Faces are oriented.
       */
      const uint32_t* m_faces {nullptr};
      uint32_t m_faceCount {0}; ///< The number of faces, i.e. one third of the size of m_faces.

      /**
       * The colors of each face.
       * @pre The buffer size must be at least m_faceCount.
       */
      const Gamebuino_Meta::ColorIndex*  m_colors {nullptr};
  };
}
