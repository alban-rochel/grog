#include "grog.h"
#include "Colors.h"

using namespace grog;
#include <cstdio>
void grog::rasterizeTriangle(const Triangle& triangle,
                             bufferType* __restrict frameBuffer) noexcept
{
  // Compute bounding boxes

  int32_t minX = min3(triangle.p1x, triangle.p2x, triangle.p3x);
  int32_t maxX = max3(triangle.p1x, triangle.p2x, triangle.p3x);
  int32_t minY = min3(triangle.p1y, triangle.p2y, triangle.p3y);
  int32_t maxY = max3(triangle.p1y, triangle.p2y, triangle.p3y);

  if(maxY < 0)
    return;
  if(maxX < 0)
    return;
  if(minX > (int32_t)screenWidth - 1)
    return;
  if(minY > (int32_t)screenHeight - 1)
    return;

  // Clip against screen bounds;

  minX = max2(minX, 0);
  minY = max2(minY, 0);
  maxX = min2(maxX, screenWidth - 1);
  maxY = min2(maxY, screenHeight - 1);

  // Rasterize

  // Triangle setup
  int _A12 = triangle.p1y - triangle.p2y, _B12 = triangle.p2x - triangle.p1x;
  int _A23 = triangle.p2y - triangle.p3y, _B23 = triangle.p3x - triangle.p2x;
  int _A31 = triangle.p3y - triangle.p1y, _B31 = triangle.p1x - triangle.p3x;

  int w1_row = orient2d(triangle.p2x, triangle.p2y, triangle.p3x, triangle.p3y, minX, minY);
  int w2_row = orient2d(triangle.p3x, triangle.p3y, triangle.p1x, triangle.p1y, minX, minY);
  int w3_row = orient2d(triangle.p1x, triangle.p1y, triangle.p2x, triangle.p2y, minX, minY);

  int w1(0), w2(0), w3(0);
  bufferType* lineStart = frameBuffer + (minY * screenWidth + minX) / 2;
  bool upperNibbleStart = (minY * screenWidth + minX) & 0x1;
  const uint8_t& col1 = (uint8_t)(triangle.color) & 0x0F;
#ifdef DITHERING_ENABLED
  bool ditheringStepStart = (minX + minY) & 0x1;
  const uint8_t& col2 = triangle.color >> 4;
#endif
  for(int y = maxY - minY + 1; y; --y, lineStart += screenWidth/2, w1_row += _B23, w2_row += _B31, w3_row += _B12)
  {
    bufferType* tmp = lineStart;
    bool upperNibble = upperNibbleStart;
#ifdef DITHERING_ENABLED
    ditheringStepStart = !ditheringStepStart;
    bool ditheringStep = ditheringStepStart;
#endif
    w1 = w1_row;
    w2 = w2_row;
    w3 = w3_row;
    /*bool lineDone(false);*/
    bool wasOk(false);
    for(int x = maxX - minX + 1; x /*&& !lineDone*/; --x, /*++tmp,*/ w1 += _A23, w2 += _A31, w3 += _A12)
    {
      if((w1|w2|w3) >= 0)
      {
#ifdef DITHERING_ENABLED
        if(upperNibble)
        {
          *tmp = (*tmp & 0x0F) | ((ditheringStep ? col1 : col2) << 4) ;
        }
        else
        {
          *tmp = (*tmp & 0xF0) | (ditheringStep ? col1 : col2) ;
        }
#else
        if(upperNibble)
        {
          *tmp = (*tmp & 0xF0) | col1;
        }
        else
        {
          *tmp = (*tmp & 0x0F) | (col1 << 4);
        }
#endif
        wasOk = true;
      }
      else if(wasOk)
      {
        //lineDone = true;
        break;
      }

      if(upperNibble)
      {
        ++tmp;
      }
      upperNibble = ! upperNibble;
#ifdef DITHERING_ENABLED
      ditheringStep = ! ditheringStep;
#endif
    }
  }
}

void oldgrog::normalize(float* io) noexcept
{
  float norm =  (io[0] * io[0]) +
      (io[1] * io[1]) +
      (io[2] * io[2]);
  float invSqrt = 1.f/sqrtf(norm);
  io[0] *= invSqrt;
  io[1] *= invSqrt;
  io[2] *= invSqrt;
}

void oldgrog::crossProd(float* left,
                     float* right,
                     float* out,
                     bool normalize) noexcept
{
  out[0] = left[1] * right[2] - right[1] * left[2];
  out[1] = left[2] * right[0] - right[2] * left[0];
  out[2] = left[0] * right[1] - right[0] * left[1];

  if(normalize)
  {
    oldgrog::normalize(out);
  }
}

void grog::normalize(int32_t* io) noexcept
{
  /*int32_t norm =  (io[0] * io[0]) +
                  (io[1] * io[1]) +
                  (io[2] * io[2]);
  float invSqrt = 1.f/(1024.f * sqrtf(norm));
  io[0] *= invSqrt;
  io[1] *= invSqrt;
  io[2] *= invSqrt;*/
  /*float norm =  ((io[0] * io[0]) +
                (io[1] * io[1]) +
                (io[2] * io[2]));*/
  float sqrtNorm = sqrtf((io[0] * io[0]) +
                          (io[1] * io[1]) +
                          (io[2] * io[2])); // =sqrt norm << 5

  float invSqrt = (1024.f)/(sqrtNorm);
  io[0] *= invSqrt;
  io[1] *= invSqrt;
  io[2] *= invSqrt;
}

void grog::crossProd(int32_t* left,
                          int32_t* right,
                          int32_t* out,
                          bool normalize) noexcept
{
  out[0] = (left[1] * right[2] - right[1] * left[2]) >> 10;
  out[1] = (left[2] * right[0] - right[2] * left[0]) >> 10;
  out[2] = (left[0] * right[1] - right[0] * left[1]) >> 10;

  if(normalize)
  {
    grog::normalize(out);
  }
}
