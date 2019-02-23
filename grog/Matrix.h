#pragma once
#include "common.h"
#include "grog.h"
#include <cmath>

namespace grog
{
  //using TransformMatrix = float[13];

  class TransformMatrix
  {
    public:
      /* [0  1  2  3 ]
       * [4  5  6  7 ]
       * [8  9  10 11]
       * [x  x  x  12]
       */
      float data[13] {1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      1};

      TransformMatrix() noexcept;
      TransformMatrix(const TransformMatrix& other) noexcept;

      TransformMatrix& identity() noexcept;
//      void leftMultiply(const TransformMatrix& left) noexcept;
//      void rightMultiply(const TransformMatrix& right) noexcept;

      TransformMatrix& rotateX(float theta) noexcept;
      TransformMatrix& rotateY(float theta) noexcept;
      TransformMatrix& rotateZ(float theta) noexcept;

      TransformMatrix& translate(float dx, float dy, float dz) noexcept;
      TransformMatrix& scale(float sx, float sy, float sz) noexcept;

      static void Product(const TransformMatrix& left,
                          const TransformMatrix& right,
                          TransformMatrix& out) noexcept;

  };

  using Matrix = float[16];
}

