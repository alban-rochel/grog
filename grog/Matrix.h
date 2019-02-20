#pragma once
#include "common.h"
#include "grog.h"
#include <cmath>

namespace grog
{
  /* [0  1  2  3 ]
   * [4  5  6  7 ]
   * [8  9  10 11]
   * [x  x  x  12]
   */
  using TransformMatrix = float[13];

  using Matrix = float[16];

  constexpr TransformMatrix Identity = { 1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 1, 0,
                                         1};

  void TMatrixProduct(const TransformMatrix& left, const TransformMatrix& right, TransformMatrix& out) noexcept;

  void TMatrixRx(TransformMatrix& io, float theta) noexcept;
  void TMatrixRy(TransformMatrix& io, float theta) noexcept;
  void TMatrixRz(TransformMatrix& io, float theta) noexcept;

  void TMatrixTranslate(TransformMatrix& io, float dx, float dy, float dz) noexcept;
  void TMatrixScale(TransformMatrix& io, float sx, float sy, float sz) noexcept;
}
