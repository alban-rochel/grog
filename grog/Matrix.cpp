#include "Matrix.h"
#include <cstring>

using namespace grog;

TransformMatrix::TransformMatrix() noexcept
{}

TransformMatrix::TransformMatrix(const TransformMatrix& other) noexcept
{
  memcpy(data, other.data, sizeof(data));
}

TransformMatrix& TransformMatrix::identity() noexcept
{
  memset(data, 0, sizeof (data));
  data[0] = data[5] = data[10] = data[12] = 1;

  return *this;
}

//void TransformMatrix::leftMultiply(const TransformMatrix &left) noexcept
//{

//}

//void TransformMatrix::rightMultiply(const TransformMatrix &left) noexcept
//{

//}

TransformMatrix& TransformMatrix::rotateX(float theta) noexcept
{
  float c(cosf(theta)),
        s(sinf(theta));

  float data_4  (data[4]),  data_5  (data[5]),
        data_6  (data[6]),  data_7  (data[7]),
        data_8  (data[8]),  data_9  (data[9]),
        data_10 (data[10]), data_11 (data[11]);

  data[4] = data_4 * c - data_8 * s;
  data[5] = data_5 * c - data_9 * s;
  data[6] = data_6 * c - data_10 * s;
  data[7] = data_7 * c - data_11 * s;

  data[8]   = data_4 * s + data_8 * c;
  data[9]   = data_5 * s + data_9 * c;
  data[10]  = data_6 * s + data_10 * c;
  data[11]  = data_7 * s + data_11 * c;

  return *this;
}

TransformMatrix& TransformMatrix::rotateY(float theta) noexcept
{
  float c(cosf(theta)),
        s(sinf(theta));

  float data_0  (data[0]),  data_1  (data[1]),
        data_2  (data[2]),  data_3  (data[3]),
        data_8  (data[8]),  data_9  (data[9]),
        data_10 (data[10]), data_11 (data[11]);

  data[0] = data_0 * c + data_8 * s;
  data[1] = data_1 * c + data_9 * s;
  data[2] = data_2 * c + data_10 * s;
  data[3] = data_3 * c + data_11 * s;

  data[8]   = - data_0 * s + data_8 * c;
  data[9]   = - data_1 * s + data_9 * c;
  data[10]  = - data_2 * s + data_10 * c;
  data[11]  = - data_3 * s + data_11 * c;

  return *this;
}

TransformMatrix& TransformMatrix::rotateZ(float theta) noexcept
{
  float c(cosf(theta)),
        s(sinf(theta));

  float data_0(data[0]), data_1(data[1]),
        data_2(data[2]), data_3(data[3]),
        data_4(data[4]), data_5(data[5]),
        data_6(data[6]), data_7(data[7]);

  data[0] = data_0 * c - data_4 * s;
  data[1] = data_1 * c - data_5 * s;
  data[2] = data_2 * c - data_6 * s;
  data[3] = data_3 * c - data_7 * s;

  data[4] = data_0 * s + data_4 * c;
  data[5] = data_1 * s + data_5 * c;
  data[6] = data_2 * s + data_6 * c;
  data[7] = data_3 * s + data_7 * c;

  return *this;
}

TransformMatrix& TransformMatrix::translate(float dx, float dy, float dz)noexcept
{
  data[3]   += dx;
  data[7]   += dy;
  data[11]  += dz;

  return *this;
}

TransformMatrix& TransformMatrix::scale(float sx, float sy, float sz) noexcept
{
  data[0]   *= sx;
  data[5]   *= sy;
  data[10]  *= sz;

  return *this;
}

/* [0  1  2  3 ]
 * [4  5  6  7 ]
 * [8  9  10 11]
 * [x  x  x  12]
 */

void TransformMatrix::Product(const TransformMatrix& left,
                              const TransformMatrix& right,
                              TransformMatrix& out) noexcept
{
#define OUT(i) out.data[i]
#define LFT(i) left.data[i]
#define RGT(i) right.data[i]
  OUT(0) = LFT(0) * RGT(0)  + LFT(1)  * RGT(4)  + LFT(2)  * RGT(8);
  OUT(1) = LFT(0) * RGT(1)  + LFT(1)  * RGT(5)  + LFT(2)  * RGT(9);
  OUT(2) = LFT(0) * RGT(2)  + LFT(1)  * RGT(6)  + LFT(2)  * RGT(10);
  OUT(3) = LFT(0) * RGT(3)  + LFT(1)  * RGT(7)  + LFT(2)  * RGT(11) + LFT(3)  * RGT(12);

  OUT(4) = LFT(4) * RGT(0)  + LFT(5)  * RGT(4)  + LFT(6)  * RGT(8);
  OUT(5) = LFT(4) * RGT(1)  + LFT(5)  * RGT(5)  + LFT(6)  * RGT(9);
  OUT(6) = LFT(4) * RGT(2)  + LFT(5)  * RGT(6)  + LFT(6)  * RGT(10);
  OUT(7) = LFT(4) * RGT(3)  + LFT(5)  * RGT(7)  + LFT(6)  * RGT(11) + LFT(7)  * RGT(12);

  OUT(8)  = LFT(8) * RGT(0)  + LFT(9)  * RGT(4)  + LFT(10)  * RGT(8);
  OUT(9)  = LFT(8) * RGT(1)  + LFT(9)  * RGT(5)  + LFT(10)  * RGT(9);
  OUT(10) = LFT(8) * RGT(2)  + LFT(9)  * RGT(6)  + LFT(10)  * RGT(10);
  OUT(11) = LFT(8) * RGT(3)  + LFT(9)  * RGT(7)  + LFT(10)  * RGT(11) + LFT(12)  * RGT(12);

  OUT(12) = LFT(12)  * RGT(12);

#undef OUT
#undef LFT
#undef RGT
}
