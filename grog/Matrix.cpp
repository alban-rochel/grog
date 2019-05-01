#include "Matrix.h"
#include "Math.h"
#include <cstring>
#include <cmath>

#ifdef __linux__
#include <iostream>
#endif

using namespace grog;

Matrix::Matrix() noexcept
{}

Matrix::Matrix(const Matrix& in_other) noexcept
{
  memcpy(m_data, in_other.m_data, sizeof(m_data));
}

Matrix& Matrix::operator=(const Matrix& in_other) noexcept
{
  memcpy(m_data, in_other.m_data, sizeof(m_data));
  return *this;
}

void Matrix::Transform(const Matrix& in_left,
                       const TransformMatrix& in_right,
                       Matrix& out_res) noexcept
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

  OUT(12) = (LFT(12) * RGT(0)  + LFT(13)  * RGT(4)  + LFT(14)  * RGT(8)) >> 10;
  OUT(13) = (LFT(12) * RGT(1)  + LFT(13)  * RGT(5)  + LFT(14)  * RGT(9)) >> 10;
  OUT(14) = (LFT(12) * RGT(2)  + LFT(13)  * RGT(6)  + LFT(14)  * RGT(10)) >> 10;
  OUT(15) = (LFT(12) * RGT(3)  + LFT(13)  * RGT(7)  + LFT(14)  * RGT(11) + LFT(15)  * RGT(12)) >> 10;

#undef OUT
#undef LFT
#undef RGT
}

Matrix Matrix::Projection(float in_fov,
                          float in_near,
                          float in_far) noexcept
{
  /*
   * glm::perspective:
   * T const rad = fovy;
    T const tanHalfFovy = tan(rad / static_cast<T>(2));

    detail::tmat4x4<T, defaultp> Result(static_cast<T>(0));
    Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
    Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
    Result[2][2] = - (zFar + zNear) / (zFar - zNear);
    Result[2][3] = - static_cast<T>(1);
    Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
    return Result;
    */

  Matrix res;
  #define GROG_INV_ASPECT 0.8f
  float invTanHalfFov = 1.f / tanf(in_fov/2.f);

  res.m_data[0]   = grog::Math::FloatToFixed(GROG_INV_ASPECT * invTanHalfFov * 40);
  res.m_data[5]   = grog::Math::FloatToFixed(invTanHalfFov * 32);
  res.m_data[10]  = grog::Math::FloatToFixed((in_far + in_near)/(in_near - in_far));
  res.m_data[11]  = grog::Math::FloatToFixed(2.f * in_far * in_near/(in_near - in_far));
  res.m_data[14]  = grog::Math::FloatToFixed(-1.f);
  res.m_data[15]  = 0;

#undef GROG_INV_ASPECT

  return res;
}

#ifdef __linux__
void Matrix::print() const noexcept
{
  std::cout <<"[\t" << m_data[0]/1024. << "\t" << m_data[1]/1024. << "\t" << m_data[2]/1024. << "\t" << m_data[3]/1024. << "\t]\n";
  std::cout <<"[\t" << m_data[4]/1024. << "\t" << m_data[5]/1024. << "\t" << m_data[6]/1024. << "\t" << m_data[7]/1024. << "\t]\n";
  std::cout <<"[\t" << m_data[8]/1024. << "\t" << m_data[9]/1024. << "\t" << m_data[10]/1024. << "\t" << m_data[11]/1024. << "\t]\n";
  std::cout <<"[\t" << m_data[12]/1024. << "\t" << m_data[13]/1024. << "\t" << m_data[14]/1024. << "\t" << m_data[15]/1024. << "\t]\n";
}
#endif
