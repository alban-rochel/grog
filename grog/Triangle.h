#pragma once

#include "grog.h"

namespace grog
{
  /**
   * @brief The Triangle is a rasterizable triangle (2D, with global Z information for paint ordering).
   */
  class Triangle
  {
    public:
      int32_t m_p1x{0}; ///< Vertex 1, X coordinate.
      int32_t m_p1y{0}; ///< Vertex 1, Y coordinate.
      int32_t m_p2x{0}; ///< Vertex 2, X coordinate.
      int32_t m_p2y{0}; ///< Vertex 2, Y coordinate.
      int32_t m_p3x{0}; ///< Vertex 3, X coordinate.
      int32_t m_p3y{0}; ///< Vertex 3, Y coordinate.
      int32_t m_z{0}; ///< Global Z ordering.
      Gamebuino_Meta::ColorIndex color{Gamebuino_Meta::ColorIndex::black}; ///< Triangle color.
      Triangle* m_next {nullptr}; ///< Next triangle in ordered list (painter's algorithm). In practice, this is the sum of the depths of the 3 vertices in the Normalized Device Coordinates cube.

      /**
       * @brief Draws the triangle.
       * @param[in] in_frameBuffer The buffer where to draw.
       * Check this page for explanation on the implementation: https://fgiesen.wordpress.com/2013/02/10/optimizing-the-basic-rasterizer/
       */
      void rasterize(uint8_t* __restrict in_frameBuffer) const noexcept;
  } ;

}
