#include "Matrix.h"

using namespace grog;

void grog::TMatrixProduct(const TransformMatrix& left, const TransformMatrix& right, TransformMatrix& out) noexcept
{
  out[0] = left[0] * right[0] + left[1] * right[4] + left[2] * right[8];
  out[1] = left[0] * right[1] + left[1] * right[5] + left[2] * right[9];
  out[2] = left[0] * right[2] + left[1] * right[6] + left[2] * right[10];
  out[3] = left[0] * right[3] + left[1] * right[7] + left[2] * right[11] + left[3] * right[12];

  out[4] = left[4] * right[0] + left[5] * right[4] + left[6] * right[8];
  out[5] = left[4] * right[1] + left[5] * right[5] + left[6] * right[9];
  out[6] = left[4] * right[2] + left[5] * right[6] + left[6] * right[10];
  out[7] = left[4] * right[3] + left[5] * right[7] + left[6] * right[11] + left[7] * right[12];

  out[8] = left[8] * right[0] + left[9] * right[4] + left[10] * right[8];
  out[9] = left[8] * right[1] + left[9] * right[5] + left[10] * right[9];
  out[10] = left[8] * right[2] + left[9] * right[6] + left[10] * right[10];
  out[11] = left[8] * right[3] + left[9] * right[4] + left[7] * right[8] + left[11] * right[12];

  out[12] = left[12] * right[12];
}

/*    [1 0 0 0]
 *    [0 c -s 0]
 *  * [0 s c 0]
 *    [0 0 0 1]
 */
void grog::TMatrixRx(TransformMatrix& io, float theta) noexcept
{
  float c(cosf(theta)),
        s(sinf(theta));

  float in_4(io[4]), in_5(io[5]),
        in_6(io[6]), in_7(io[7]),
        in_8(io[8]), in_9(io[9]),
        in_10(io[10]), in_11(io[11]);

  io[4] = in_4 * c - in_8 * s;
  io[5] = in_5 * c - in_9 * s;
  io[6] = in_6 * c - in_10 * s;
  io[7] = in_7 * c - in_11 * s;

  io[8] = in_4 * s + in_8 * c;
  io[9] = in_5 * s + in_9 * c;
  io[10] = in_6 * s + in_10 * c;
  io[11] = in_7 * s + in_11 * c;
}

/*    [c 0 s 0]
 *    [0 1 0 0]
 *  * [-s 0 c 0]
 *    [0 0 0 1]
 */
void grog::TMatrixRy(TransformMatrix& io, float theta) noexcept
{
  float c(cosf(theta)),
        s(sinf(theta));

  float in_0(io[0]), in_1(io[1]),
      in_2(io[2]), in_3(io[3]),
      in_8(io[8]), in_9(io[9]),
      in_10(io[10]), in_11(io[11]);

  io[0] = in_0 * c + in_8 * s;
  io[1] = in_1 * c + in_9 * s;
  io[2] = in_2 * c + in_10 * s;
  io[3] = in_3 * c + in_11 * s;

  io[8] = - in_0 * s + in_8 * c;
  io[9] = - in_1 * s + in_9 * c;
  io[10] = - in_2 * s + in_10 * c;
  io[11] = - in_3 * s + in_11 * c;
}

/*    [c -s 0 0]
 *    [s c 0 0]
 *  * [0 0 1 0]
 *    [0 0 0 1]
 */
void grog::TMatrixRz(TransformMatrix& io, float theta) noexcept
{
  float c(cosf(theta)),
        s(sinf(theta));

  float in_0(io[0]), in_1(io[1]),
        in_2(io[2]), in_3(io[3]),
        in_4(io[4]), in_5(io[5]),
        in_6(io[6]), in_7(io[7]);

  io[0] = in_0 * c - in_4 * s;
  io[1] = in_1 * c - in_5 * s;
  io[2] = in_2 * c - in_6 * s;
  io[3] = in_3 * c - in_7 * s;

  io[4] = in_0 * s + in_4 * c;
  io[5] = in_1 * s + in_5 * c;
  io[6] = in_2 * s + in_6 * c;
  io[7] = in_3 * s + in_7 * c;
}

void grog::TMatrixTranslate(TransformMatrix& io, float dx, float dy, float dz) noexcept
{
  io[3] += dx;
  io[7] += dy;
  io[11] += dz;
}

void grog::TMatrixScale(TransformMatrix& io, float sx, float sy, float sz) noexcept
{
  io[0] *= sx;
  io[5] *= sy;
  io[10] *= sz;
}
