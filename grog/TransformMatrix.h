#pragma once

#include "grog.h"

namespace grog
{
  /**
   * @brief 4*4 transform matrix of signed Q21.10 fixed point.
   * Transform matrices always have the first 3 values of the 4th row set to 0.
   * We use this particularity to save memory and CPU cycles.
   * Stored as row-major order.
   */
  class TransformMatrix
  {
    public:
      /* [0  1  2  3 ]
       * [4  5  6  7 ]
       * [8  9  10 11]
       * [x  x  x  12]
       */
      /**
       * @brief The actual content, initialized as the identity matrix.
       */
      int32_t m_data[13] {1 << 10,  0,        0,        0,
                          0,        1 << 10,  0,        0,
                          0,        0,        1 << 10,  0,
                                                        1 << 10};

      /**
       * @brief Constructor, creates an identity matrix.
       */
      TransformMatrix() noexcept;
      /**
       * @brief Copy constructor.
       * @param[in] in_other The TransformMatrix to copy.
       */
      TransformMatrix(const TransformMatrix& in_other) noexcept;

      /**
       * @brief Assignment operator.
       * @param[in] in_other The TransformMatrix to copy
       * @return Self.
       * @pre No self-assignment.
       */
      TransformMatrix& operator=(const TransformMatrix& in_other) noexcept;

      /**
       * @brief Sets as the identity matrix.
       * @return Self.
       */
      TransformMatrix& identity() noexcept;

      /**
       * @brief Rotates around the X axis.
       * @param[in] in_theta The angle, as 1024th of \f$2*\pi\f$.
       * @return Self.
       */
      TransformMatrix& rotateX(int32_t in_theta) noexcept;
      /**
       * @brief Rotates around the Y axis.
       * @param[in] in_theta The angle, as 1024th of \f$2*\pi\f$.
       * @return Self.
       */
      TransformMatrix& rotateY(int32_t in_theta) noexcept;
      /**
       * @brief Rotates around the Z axis.
       * @param[in] in_theta The angle, as 1024th of \f$2*\pi\f$.
       * @return Self.
       */
      TransformMatrix& rotateZ(int32_t in_theta) noexcept;

      /**
       * @brief Translates the transformation.
       * @param in_dx The X offset.
       * @param in_dy The Y offset.
       * @param in_dz The Z offset.
       * @return Self
       */
      TransformMatrix& translate(int32_t in_dx, int32_t in_dy, int32_t in_dz) noexcept;

      /**
       * @brief Scales the transformation.
       * @param in_sx The X scale factor.
       * @param in_sy The Y scale factor.
       * @param in_sz The Z scale factor.
       * @return Self
       */
      TransformMatrix& scale(int32_t in_sx, int32_t in_sy, int32_t in_sz) noexcept;

      /**
       * @brief Product of two TransformMatrices.
       * @param[in] in_left The left TransformMatrix.
       * @param[in] in_right The right TransformMatrix.
       * @param[out] out_res The result, as a TransformMatrix.
       */
      static void Product(const TransformMatrix& in_left,
                          const TransformMatrix& in_right,
                          TransformMatrix& out_res) noexcept;

      /**
       * @brief Creates a View TransformMatrix, which transforms the world coordinates in a projectable coordinates system.
       * @param[in] in_eyeX The camera X position.
       * @param[in] in_eyeY The camera Y position.
       * @param[in] in_eyeZ The camera Z position.
       * @param[in] in_centerX The X coordinate of the point targetted by the camera.
       * @param[in] in_centerY The Y coordinate of the point targetted by the camera.
       * @param[in] in_centerZ The Z coordinate of the point targetted by the camera.
       * @param[in] in_upX The X coordinates of the Up vector.
       * @param[in] in_upY The Y coordinates of the Up vector.
       * @param[in] in_upZ The Z coordinates of the Up vector.
       * @return A View TransformMatrix.
       * Implementation based on glm::lookat (https://glm.g-truc.net/0.9.4/api/a00151.html#gae2dca3785b6d5796e876114af58a60a1).
       */
      static TransformMatrix View(int32_t in_eyeX, int32_t in_eyeY, int32_t in_eyeZ,
                                  int32_t in_centerX, int32_t in_centerY, int32_t in_centerZ,
                                  int32_t in_upX, int32_t in_upY, int32_t in_upZ) noexcept;

#ifdef __linux__
      void print() const noexcept;
#endif

  };
}
