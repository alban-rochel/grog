#include "Matrix.h"
#include "trigo.h"
#include <cstring>

#ifdef __linux__
#include <iostream>
#endif

using namespace fixedgrog;

TransformMatrix::TransformMatrix() noexcept
{}

TransformMatrix::TransformMatrix(const TransformMatrix& other) noexcept
{
  memcpy(data, other.data, sizeof(data));
}

TransformMatrix& TransformMatrix::operator=(const TransformMatrix& other) noexcept
{
  memcpy(data, other.data, sizeof(data));
  return *this;
}

TransformMatrix& TransformMatrix::identity() noexcept
{
  memset(data, 0, sizeof (data));
  data[0] = data[5] = data[10] = data[12] = 1;

  return *this;
}

TransformMatrix& TransformMatrix::rotateX(int32_t theta) noexcept
{
  int32_t c(Cos(theta)),
          s(Sin(theta));

  int32_t data_4  (data[4]),  data_5  (data[5]),
          data_6  (data[6]),  data_7  (data[7]),
          data_8  (data[8]),  data_9  (data[9]),
          data_10 (data[10]), data_11 (data[11]);

  data[4] = (data_4 * c - data_8 * s) >> 10;
  data[5] = (data_5 * c - data_9 * s) >> 10;
  data[6] = (data_6 * c - data_10 * s) >> 10;
  data[7] = (data_7 * c - data_11 * s) >> 10;

  data[8]   = (data_4 * s + data_8 * c) >> 10;
  data[9]   = (data_5 * s + data_9 * c) >> 10;
  data[10]  = (data_6 * s + data_10 * c) >> 10;
  data[11]  = (data_7 * s + data_11 * c) >> 10;

  return *this;
}

TransformMatrix& TransformMatrix::rotateY(int32_t theta) noexcept
{
  int32_t c(Cos(theta)),
          s(Sin(theta));

  int32_t data_0  (data[0]),  data_1  (data[1]),
          data_2  (data[2]),  data_3  (data[3]),
          data_8  (data[8]),  data_9  (data[9]),
          data_10 (data[10]), data_11 (data[11]);

  data[0] = (data_0 * c + data_8 * s) >> 10;
  data[1] = (data_1 * c + data_9 * s) >> 10;
  data[2] = (data_2 * c + data_10 * s) >> 10;
  data[3] = (data_3 * c + data_11 * s) >> 10;

  data[8]   = (- data_0 * s + data_8 * c) >> 10;
  data[9]   = (- data_1 * s + data_9 * c) >> 10;
  data[10]  = (- data_2 * s + data_10 * c) >> 10;
  data[11]  = (- data_3 * s + data_11 * c) >> 10;

  return *this;
}

TransformMatrix& TransformMatrix::rotateZ(int32_t theta) noexcept
{
  int32_t c(Cos(theta)),
          s(Sin(theta));

  int32_t data_0(data[0]), data_1(data[1]),
          data_2(data[2]), data_3(data[3]),
          data_4(data[4]), data_5(data[5]),
          data_6(data[6]), data_7(data[7]);

  data[0] = (data_0 * c - data_4 * s) >> 10;
  data[1] = (data_1 * c - data_5 * s) >> 10;
  data[2] = (data_2 * c - data_6 * s) >> 10;
  data[3] = (data_3 * c - data_7 * s) >> 10;

  data[4] = (data_0 * s + data_4 * c) >> 10;
  data[5] = (data_1 * s + data_5 * c) >> 10;
  data[6] = (data_2 * s + data_6 * c) >> 10;
  data[7] = (data_3 * s + data_7 * c) >> 10;

  return *this;
}

TransformMatrix& TransformMatrix::translate(int32_t dx, int32_t dy, int32_t dz)noexcept
{
  data[3]   += dx;
  data[7]   += dy;
  data[11]  += dz;

  return *this;
}

TransformMatrix& TransformMatrix::scale(int32_t sx, int32_t sy, int32_t sz) noexcept
{
  data[0]   *= sx; data[0]  = data[0] >> 10;
  data[5]   *= sy; data[5]  = data[5] >> 10;
  data[10]  *= sz; data[10] = data[10] >> 10;

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
  OUT(11) = (LFT(8) * RGT(3)  + LFT(9)  * RGT(7)  + LFT(10)  * RGT(11) + LFT(12)  * RGT(12)) >> 10;

  OUT(12) = (LFT(12)  * RGT(12)) >> 10;

#undef OUT
#undef LFT
#undef RGT
}

TransformMatrix TransformMatrix::View(int32_t eyeX, int32_t eyeY, int32_t eyeZ,
                                      int32_t centerX, int32_t centerY, int32_t centerZ,
                                      int32_t upX, int32_t upY, int32_t upZ) noexcept
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
  // y vector starts at res.data[4]
  res.data[4] = upX;
  res.data[5] = upY;
  res.data[6] = upZ;

  // z vector starts at res.data[8];
  res.data[8] = eyeX - centerX;
  res.data[9] = eyeY - centerY;
  res.data[10] = eyeZ - centerZ;

  normalize(res.data + 8);
  // z is normalized

  // x = y(up) cross z
  crossProd(res.data + 4 /*y*/,
            res.data + 8 /*z*/,
            res.data /*x*/,
            true);
  // x is normalized

  //recompute up = z cross x

  crossProd(res.data + 8 /*z*/,
            res.data /*x*/,
            res.data + 4 /*y*/,
            false /* no need to normalize */);

  // translate to origin

  //res.translate(-eyeX, -eyeY, -eyeZ);
  res.data[3] = - res.data[0] * eyeX
                - res.data[1] * eyeY
                - res.data[2] * eyeZ;
  res.data[7] = - res.data[4] * eyeX
                - res.data[5] * eyeY
                - res.data[6] * eyeZ;
  res.data[11] =  - res.data[8] * eyeX
                  - res.data[9] * eyeY
                  - res.data[10] * eyeZ;

  return res;
}

#ifdef __linux__
void TransformMatrix::print() noexcept
{
  std::cout <<"[\t" << data[0]/1024. << "\t" << data[1]/1024. << "\t" << data[2]/1024. << "\t" << data[3]/1024. << "\t]\n";
  std::cout <<"[\t" << data[4]/1024. << "\t" << data[5]/1024. << "\t" << data[6]/1024. << "\t" << data[7]/1024. << "\t]\n";
  std::cout <<"[\t" << data[8]/1024. << "\t" << data[9]/1024. << "\t" << data[10]/1024. << "\t" << data[11]/1024. << "\t]\n";
  std::cout <<"[\tx\tx\tx\t" << data[12]/1024. << "\t]\n";
}
#endif

//Matrix::Matrix() noexcept
//{}

//Matrix::Matrix(const Matrix& other) noexcept
//{
//  memcpy(data, other.data, sizeof(data));
//}

//Matrix& Matrix::operator=(const Matrix& other) noexcept
//{
//  memcpy(data, other.data, sizeof(data));
//  return *this;
//}

//void Matrix::Transform(const Matrix& left,
//                       const TransformMatrix& right,
//                       Matrix& out) noexcept
//{
//#define OUT(i) out.data[i]
//#define LFT(i) left.data[i]
//#define RGT(i) right.data[i]
//  OUT(0) = LFT(0) * RGT(0)  + LFT(1)  * RGT(4)  + LFT(2)  * RGT(8);
//  OUT(1) = LFT(0) * RGT(1)  + LFT(1)  * RGT(5)  + LFT(2)  * RGT(9);
//  OUT(2) = LFT(0) * RGT(2)  + LFT(1)  * RGT(6)  + LFT(2)  * RGT(10);
//  OUT(3) = LFT(0) * RGT(3)  + LFT(1)  * RGT(7)  + LFT(2)  * RGT(11) + LFT(3)  * RGT(12);

//  OUT(4) = LFT(4) * RGT(0)  + LFT(5)  * RGT(4)  + LFT(6)  * RGT(8);
//  OUT(5) = LFT(4) * RGT(1)  + LFT(5)  * RGT(5)  + LFT(6)  * RGT(9);
//  OUT(6) = LFT(4) * RGT(2)  + LFT(5)  * RGT(6)  + LFT(6)  * RGT(10);
//  OUT(7) = LFT(4) * RGT(3)  + LFT(5)  * RGT(7)  + LFT(6)  * RGT(11) + LFT(7)  * RGT(12);

//  OUT(8)  = LFT(8) * RGT(0)  + LFT(9)  * RGT(4)  + LFT(10)  * RGT(8);
//  OUT(9)  = LFT(8) * RGT(1)  + LFT(9)  * RGT(5)  + LFT(10)  * RGT(9);
//  OUT(10) = LFT(8) * RGT(2)  + LFT(9)  * RGT(6)  + LFT(10)  * RGT(10);
//  OUT(11) = LFT(8) * RGT(3)  + LFT(9)  * RGT(7)  + LFT(10)  * RGT(11) + LFT(12)  * RGT(12);

//  OUT(12) = LFT(12) * RGT(0)  + LFT(13)  * RGT(4)  + LFT(14)  * RGT(8);
//  OUT(13) = LFT(12) * RGT(1)  + LFT(13)  * RGT(5)  + LFT(14)  * RGT(9);
//  OUT(14) = LFT(12) * RGT(2)  + LFT(13)  * RGT(6)  + LFT(14)  * RGT(10);
//  OUT(15) = LFT(12) * RGT(3)  + LFT(13)  * RGT(7)  + LFT(14)  * RGT(11) + LFT(15)  * RGT(12);

//#undef OUT
//#undef LFT
//#undef RGT
//}

//Matrix Matrix::Projection(float fov,
//                          float near,
//                          float far) noexcept
//{
//  /*
//   * glm::perspective:
//   * T const rad = fovy;
//    T const tanHalfFovy = tan(rad / static_cast<T>(2));

//    detail::tmat4x4<T, defaultp> Result(static_cast<T>(0));
//    Result[0][0] = static_cast<T>(1) / (aspect * tanHalfFovy);
//    Result[1][1] = static_cast<T>(1) / (tanHalfFovy);
//    Result[2][2] = - (zFar + zNear) / (zFar - zNear);
//    Result[2][3] = - static_cast<T>(1);
//    Result[3][2] = - (static_cast<T>(2) * zFar * zNear) / (zFar - zNear);
//    return Result;
//    */

//  Matrix res;
//#define GROG_PI_OVER_360 0.0087266462599716477f
////#define GROG_INV_ASPECT 1.25f
//  #define GROG_INV_ASPECT 0.8f
//  float invTanHalfFov = 1.f / tanf(fov/2.f);

//  res.data[0]   = GROG_INV_ASPECT * invTanHalfFov;
//  res.data[5]   = invTanHalfFov;
//  res.data[10]  = (far + near)/(near - far);
//  res.data[11]  = 2.f * far * near/(near - far);
//  res.data[14]  = -1;
//  res.data[15]  = 0.f;

//#undef GROG_INV_ASPECT
//#undef GROG_PI_OVER_360


//  return res;
//}

//#ifdef __linux__
//void Matrix::print() noexcept
//{
//  std::cout <<"[\t" << data[0] << "\t" << data[1] << "\t" << data[2] << "\t" << data[3] << "\t]\n";
//  std::cout <<"[\t" << data[4] << "\t" << data[5] << "\t" << data[6] << "\t" << data[7] << "\t]\n";
//  std::cout <<"[\t" << data[8] << "\t" << data[9] << "\t" << data[10] << "\t" << data[11] << "\t]\n";
//  std::cout <<"[\t" << data[12] << "\t" << data[13] << "\t" << data[14] << "\t" << data[15] << "\t]\n";
//}
//#endif
