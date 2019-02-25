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

      //pre: no self-assignment
      TransformMatrix& operator=(const TransformMatrix& other) noexcept;

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

      static TransformMatrix View(float eyeX, float eyeY, float eyeZ,
                                  float centerX, float centerY, float centerZ,
                                  float upX, float upY, float upZ) noexcept;

#ifdef __linux__
      void print() noexcept;
#endif

  };

  class Matrix
  {
    public:
      float data[16] {1, 0, 0, 0,
                      0, 1, 0, 0,
                      0, 0, 1, 0,
                      0, 0, 0, 1};

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
      void print() noexcept;
#endif
  };

}

