#include "Math.h"

using namespace grog;

void Math::Normalize(int32_t* io) noexcept
{
  float sqrtNorm = sqrtf((io[0] * io[0]) +
                          (io[1] * io[1]) +
                          (io[2] * io[2])); // =sqrt norm << 5

  float invSqrt = (1024.f)/(sqrtNorm);
  io[0] *= invSqrt;
  io[1] *= invSqrt;
  io[2] *= invSqrt;
}

void Math::CrossProd(int32_t* left,
                          int32_t* right,
                          int32_t* out,
                          bool normalize) noexcept
{
  out[0] = (left[1] * right[2] - right[1] * left[2]) >> 10;
  out[1] = (left[2] * right[0] - right[2] * left[0]) >> 10;
  out[2] = (left[0] * right[1] - right[0] * left[1]) >> 10;

  if(normalize)
  {
    Normalize(out);
  }
}
