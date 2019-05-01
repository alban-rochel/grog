#include "Matrix.h"
#include "Math.h"
#include <cstring>
#include <cmath>

#ifdef __linux__
#include <iostream>
#endif

using namespace grog;

TransformMatrix::TransformMatrix() noexcept
{}

TransformMatrix::TransformMatrix(const TransformMatrix& in_other) noexcept
{
  memcpy(m_data, in_other.m_data, sizeof(m_data));
}

TransformMatrix& TransformMatrix::operator=(const TransformMatrix& in_other) noexcept
{
  memcpy(m_data, in_other.m_data, sizeof(m_data));
  return *this;
}

TransformMatrix& TransformMatrix::identity() noexcept
{
  memset(m_data, 0, sizeof (m_data));
  m_data[0] = m_data[5] = m_data[10] = m_data[12] = (1 << 10);

  return *this;
}

TransformMatrix& TransformMatrix::rotateX(int32_t in_theta) noexcept
{
  int32_t c(Math::Cos(in_theta)),
          s(Math::Sin(in_theta));

  int32_t data_4  (m_data[4]),  data_5  (m_data[5]),
          data_6  (m_data[6]),  data_7  (m_data[7]),
          data_8  (m_data[8]),  data_9  (m_data[9]),
          data_10 (m_data[10]), data_11 (m_data[11]);

  m_data[4] = (data_4 * c - data_8 * s) >> 10;
  m_data[5] = (data_5 * c - data_9 * s) >> 10;
  m_data[6] = (data_6 * c - data_10 * s) >> 10;
  m_data[7] = (data_7 * c - data_11 * s) >> 10;

  m_data[8]   = (data_4 * s + data_8 * c) >> 10;
  m_data[9]   = (data_5 * s + data_9 * c) >> 10;
  m_data[10]  = (data_6 * s + data_10 * c) >> 10;
  m_data[11]  = (data_7 * s + data_11 * c) >> 10;

  return *this;
}

TransformMatrix& TransformMatrix::rotateY(int32_t in_theta) noexcept
{
  int32_t c(Math::Cos(in_theta)),
          s(Math::Sin(in_theta));

  int32_t data_0  (m_data[0]),  data_1  (m_data[1]),
          data_2  (m_data[2]),  data_3  (m_data[3]),
          data_8  (m_data[8]),  data_9  (m_data[9]),
          data_10 (m_data[10]), data_11 (m_data[11]);

  m_data[0] = (data_0 * c + data_8 * s) >> 10;
  m_data[1] = (data_1 * c + data_9 * s) >> 10;
  m_data[2] = (data_2 * c + data_10 * s) >> 10;
  m_data[3] = (data_3 * c + data_11 * s) >> 10;

  m_data[8]   = (- data_0 * s + data_8 * c) >> 10;
  m_data[9]   = (- data_1 * s + data_9 * c) >> 10;
  m_data[10]  = (- data_2 * s + data_10 * c) >> 10;
  m_data[11]  = (- data_3 * s + data_11 * c) >> 10;

  return *this;
}

TransformMatrix& TransformMatrix::rotateZ(int32_t in_theta) noexcept
{
  int32_t c(Math::Cos(in_theta)),
          s(Math::Sin(in_theta));

  int32_t data_0(m_data[0]), data_1(m_data[1]),
          data_2(m_data[2]), data_3(m_data[3]),
          data_4(m_data[4]), data_5(m_data[5]),
          data_6(m_data[6]), data_7(m_data[7]);

  m_data[0] = (data_0 * c - data_4 * s) >> 10;
  m_data[1] = (data_1 * c - data_5 * s) >> 10;
  m_data[2] = (data_2 * c - data_6 * s) >> 10;
  m_data[3] = (data_3 * c - data_7 * s) >> 10;

  m_data[4] = (data_0 * s + data_4 * c) >> 10;
  m_data[5] = (data_1 * s + data_5 * c) >> 10;
  m_data[6] = (data_2 * s + data_6 * c) >> 10;
  m_data[7] = (data_3 * s + data_7 * c) >> 10;

  return *this;
}

TransformMatrix& TransformMatrix::translate(int32_t in_dx, int32_t in_dy, int32_t in_dz)noexcept
{
  m_data[3]   += in_dx;
  m_data[7]   += in_dy;
  m_data[11]  += in_dz;

  return *this;
}

TransformMatrix& TransformMatrix::scale(int32_t in_sx, int32_t in_sy, int32_t in_sz) noexcept
{
  m_data[0]   *= in_sx; m_data[0]  = m_data[0] >> 10;
  m_data[5]   *= in_sy; m_data[5]  = m_data[5] >> 10;
  m_data[10]  *= in_sz; m_data[10] = m_data[10] >> 10;

  return *this;
}

void TransformMatrix::Product(const TransformMatrix& in_left,
                              const TransformMatrix& in_right,
                              TransformMatrix& out_res) noexcept
{
#define OUT(i) out_res.m_data[i]
#define LFT(i) in_left.m_data[i]
#define RGT(i) in_right.m_data[i]
  OUT(0) = (LFT(0) * RGT(0)  + LFT(1)  * RGT(4)  + LFT(2)  * RGT(8)) >> 10;
  OUT(1) = (LFT(0) * RGT(1)  + LFT(1)  * RGT(5)  + LFT(2)  * RGT(9)) >> 10;
  OUT(2) = (LFT(0) * RGT(2)  + LFT(1)  * RGT(6)  + LFT(2)  * RGT(10)) >> 10;
  OUT(3) = (LFT(0) * RGT(3)  + LFT(1)  * RGT(7)  + LFT(2)  * RGT(11) + LFT(3)  * RGT(12)) >> 10;

  OUT(4) = (LFT(4) * RGT(0)  + LFT(5)  * RGT(4)  + LFT(6)  * RGT(8)) >> 10;
  OUT(5) = (LFT(4) * RGT(1)  + LFT(5)  * RGT(5)  + LFT(6)  * RGT(9)) >> 10;
  OUT(6) = (LFT(4) * RGT(2)  + LFT(5)  * RGT(6)  + LFT(6)  * RGT(10)) >> 10;
  OUT(7) = (LFT(4) * RGT(3)  + LFT(5)  * RGT(7)  + LFT(6)  * RGT(11) + LFT(7)  * RGT(12)) >> 10;

  OUT(8)  = (LFT(8) * RGT(0)  + LFT(9)  * RGT(4)  + LFT(10)  * RGT(8)) >> 10;
  OUT(9)  = (LFT(8) * RGT(1)  + LFT(9)  * RGT(5)  + LFT(10)  * RGT(9)) >> 10;
  OUT(10) = (LFT(8) * RGT(2)  + LFT(9)  * RGT(6)  + LFT(10)  * RGT(10)) >> 10;
  OUT(11) = (LFT(8) * RGT(3)  + LFT(9)  * RGT(7)  + LFT(10)  * RGT(11) + LFT(11)  * RGT(12)) >> 10;

  OUT(12) = (LFT(12)  * RGT(12)) >> 10;

#undef OUT
#undef LFT
#undef RGT
}

TransformMatrix TransformMatrix::View(int32_t in_eyeX, int32_t in_eyeY, int32_t in_eyeZ,
                                      int32_t in_centerX, int32_t in_centerY, int32_t in_centerZ,
                                      int32_t in_upX, int32_t in_upY, int32_t in_upZ) noexcept
{

  /*
   glm::lookAt
    detail::tvec3<T, P> const f(normalize(center - eye));
    detail::tvec3<T, P> const s(normalize(cross(f, up)));
    detail::tvec3<T, P> const u(cross(s, f));

    detail::tmat4x4<T, P> Result(1);
    Result[0][0] = s.x;
    Result[1][0] = s.y;
    Result[2][0] = s.z;
    Result[0][1] = u.x;
    Result[1][1] = u.y;
    Result[2][1] = u.z;
    Result[0][2] =-f.x;
    Result[1][2] =-f.y;
    Result[2][2] =-f.z;
    Result[3][0] =-dot(s, eye);
    Result[3][1] =-dot(u, eye);
    Result[3][2] = dot(f, eye);
    return Result;
   */
  TransformMatrix res;
  // y vector starts at res.m_data[4]
  res.m_data[4] = in_upX;
  res.m_data[5] = in_upY;
  res.m_data[6] = in_upZ;

  // z vector starts at res.m_data[8];
  res.m_data[8]   = in_eyeX - in_centerX;
  res.m_data[9]   = in_eyeY - in_centerY;
  res.m_data[10]  = in_eyeZ - in_centerZ;

  Math::Normalize(res.m_data + 8);
  // z is normalized

  // x = y(up) cross z
  Math::CrossProd(res.m_data + 4 /*y*/,
                  res.m_data + 8 /*z*/,
                  res.m_data /*x*/,
                  true);
  // x is normalized

  //recompute up = z cross x

  Math::CrossProd(res.m_data + 8 /*z*/,
                  res.m_data /*x*/,
                  res.m_data + 4 /*y*/,
                  false /* no need to normalize */);

  // translate to origin

  //res.translate(-eyeX, -eyeY, -eyeZ);
  res.m_data[3] = ( - res.m_data[0]   * in_eyeX
                    - res.m_data[1]   * in_eyeY
                    - res.m_data[2]   * in_eyeZ) >> 10;
  res.m_data[7] = ( - res.m_data[4]   * in_eyeX
                    - res.m_data[5]   * in_eyeY
                    - res.m_data[6]   * in_eyeZ) >> 10;
  res.m_data[11] = (- res.m_data[8]   * in_eyeX
                    - res.m_data[9]   * in_eyeY
                    - res.m_data[10]  * in_eyeZ) >> 10;

  return res;
}

#ifdef __linux__
void TransformMatrix::print() const noexcept
{
  std::cout <<"[\t" << m_data[0]/1024. << "\t" << m_data[1]/1024. << "\t" << m_data[2]/1024. << "\t" << m_data[3]/1024. << "\t]\n";
  std::cout <<"[\t" << m_data[4]/1024. << "\t" << m_data[5]/1024. << "\t" << m_data[6]/1024. << "\t" << m_data[7]/1024. << "\t]\n";
  std::cout <<"[\t" << m_data[8]/1024. << "\t" << m_data[9]/1024. << "\t" << m_data[10]/1024. << "\t" << m_data[11]/1024. << "\t]\n";
  std::cout <<"[\tx\tx\tx\t" << m_data[12]/1024. << "\t]\n";
}
#endif

