#include "Math.h"
#include <cmath>

using namespace grog;

void Math::Normalize(int32_t* io_vect) noexcept
{
  float sqrtNorm = sqrtf( (io_vect[0] * io_vect[0]) +
                          (io_vect[1] * io_vect[1]) +
                          (io_vect[2] * io_vect[2]));

  float invSqrt = (1024.f)/(sqrtNorm);
  io_vect[0] *= invSqrt;
  io_vect[1] *= invSqrt;
  io_vect[2] *= invSqrt;
}

void Math::CrossProd( int32_t* in_left,
                      int32_t* in_right,
                      int32_t* out_vect,
                      bool in_normalize) noexcept
{
  out_vect[0] = (in_left[1] * in_right[2] - in_right[1] * in_left[2]) >> 10;
  out_vect[1] = (in_left[2] * in_right[0] - in_right[2] * in_left[0]) >> 10;
  out_vect[2] = (in_left[0] * in_right[1] - in_right[0] * in_left[1]) >> 10;

  if(in_normalize)
  {
    Normalize(out_vect);
  }
}
