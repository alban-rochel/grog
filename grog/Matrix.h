#pragma once
#include "common.h"
#include "grog.h"
#include <cmath>

namespace grog
{
  class TransformMatrix
  {
    public:
      /* [0  1  2  3 ]
       * [4  5  6  7 ]
       * [8  9  10 11]
       * [x  x  x  12]
       */
      int32_t data[13] {1 << 10, 0, 0, 0,
                        0, 1 << 10, 0, 0,
                        0, 0, 1 << 10, 0,
                        1 << 10};

      TransformMatrix() noexcept;
      TransformMatrix(const TransformMatrix& other) noexcept;

      //pre: no self-assignment
      TransformMatrix& operator=(const TransformMatrix& other) noexcept;

      TransformMatrix& identity() noexcept;

      TransformMatrix& rotateX(int32_t theta) noexcept;
      TransformMatrix& rotateY(int32_t theta) noexcept;
      TransformMatrix& rotateZ(int32_t theta) noexcept;

      TransformMatrix& translate(int32_t dx, int32_t dy, int32_t dz) noexcept;
      TransformMatrix& scale(int32_t sx, int32_t sy, int32_t sz) noexcept;

      static void Product(const TransformMatrix& left,
                          const TransformMatrix& right,
                          TransformMatrix& out) noexcept;

      static TransformMatrix View(int32_t eyeX, int32_t eyeY, int32_t eyeZ,
                                  int32_t centerX, int32_t centerY, int32_t centerZ,
                                  int32_t upX, int32_t upY, int32_t upZ) noexcept;

#ifdef __linux__
      void print() const noexcept;
#endif

  };

  class Matrix
  {
    public:
      int32_t data[16] {1 << 10, 0, 0, 0,
                        0, 1 << 10, 0, 0,
                        0, 0, 1 << 10, 0,
                        0, 0, 0, 1 << 10};

      Matrix() noexcept;
      Matrix(const Matrix& other) noexcept;

      // pre no self assignment
      Matrix& operator=(const Matrix& other) noexcept;

      static void Transform(const Matrix& left,
                            const TransformMatrix& right,
                            Matrix& out) noexcept;

      static Matrix Projection(float fov,
                               float near,
                               float far) noexcept;

#ifdef __linux__
      void print() const noexcept;
#endif
  };
}
