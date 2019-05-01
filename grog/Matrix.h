#pragma once

#include "grog.h"
#include "TransformMatrix.h"

namespace grog
{
  /**
   * @brief Generic 4*4 of signed Q21.10 fixed point.
   * Stored as row-major order.
   */
  class Matrix
  {
    public:
      /**
       * @brief The actual content, initialized as the identity matrix.
       */
      int32_t m_data[16] {1 << 10, 0, 0, 0,
                          0, 1 << 10, 0, 0,
                          0, 0, 1 << 10, 0,
                          0, 0, 0, 1 << 10};

      /**
       * @brief Constructor.
       */
      Matrix() noexcept = default;

      /**
       * @brief Copy constructor.
       * @param[in] in_other The Matrix to copy.
       */
      Matrix(const Matrix& in_other) noexcept;

      /**
       * @brief Assignment operator.
       * @param[in] in_other The Matrix to copy
       * @return Self.
       * @pre No self-assignment.
       */
      Matrix& operator=(const Matrix& in_other) noexcept;

      /**
       * @brief Product of a Matrix (left) with a TransformMatrix (right).
       * @param[in] in_left The Matrix.
       * @param[in] in_right The TransformMatrix.
       * @param[out] out_res The result, as a Matrix.
       */
      static void Transform(const Matrix& in_left,
                            const TransformMatrix& in_right,
                            Matrix& out_res) noexcept;

      /**
       * @brief Creates a perspective projection Matrix.
       * Check http://www.songho.ca/opengl/gl_projectionmatrix.html to better understand how this is constructed.
       * @param[in] in_fov The horizontal field of view, in radians. The vertical field is inferred knowing the Gamebuino Meta display ratio.
       * @param[in] in_near The distance to the near plane.
       * @param[in] in_far The distance to the far plane.
       * @return A perspective projection Matrix.
       * Implementation based on glm::perspective (https://glm.g-truc.net/0.9.4/api/a00151.html#ga283629a5ac7fb9037795435daf22560f).
       */
      static Matrix Projection(float in_fov,
                               float in_near,
                               float in_far) noexcept;

#ifdef __linux__
      void print() const noexcept;
#endif
  };
}
