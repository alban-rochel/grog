#include "grog.h"

using namespace grog;

#if 0
void grog::rasterizeTriangle(const int* __restrict v0,
                  const int* __restrict v1,
                  const int* __restrict v2,
                  const bufferType& color,
                  bufferType* __restrict frameBuffer) noexcept
{
    const int& v0x = *(v0++);
    const int& v0y = *(v0++);

    const int& v1x = *(v1++);
    const int& v1y = *(v1++);

    const int& v2x = *(v2++);
    const int& v2y = *(v2++);

    // Compute bounding boxes

    int minX = min3(v0x, v1x, v2x);
    int minY = min3(v0y, v1y, v2y);
    int maxX = max3(v0x, v1x, v2x);
    int maxY = max3(v0y, v1y, v2y);

    // Clip against screen bounds;

    minX = max2(minX, 0);
    minY = max2(minY, 0);
    maxX = min2(maxX, screenWidth - 1);
    maxY = min2(maxY, screenHeight - 1);

    // Rasterize

    // Triangle setup
    int _A01 = v0y - v1y, _B01 = v1x - v0x;
    int _A12 = v1y - v2y, _B12 = v2x - v1x;
    int _A20 = v2y - v0y, _B20 = v0x - v2x;

    int w0_row = orient2d(v1x, v1y, v2x, v2y, minX, minY);
    int w1_row = orient2d(v2x, v2y, v0x, v0y, minX, minY);
    int w2_row = orient2d(v0x, v0y, v1x, v1y, minX, minY);

    int w0(0), w1(0), w2(0);
    bufferType* lineStart = frameBuffer + minY * screenWidth + minX;
    for(int y = maxY - minY + 1; y; --y, lineStart += screenWidth, w0_row += _B12, w1_row += _B20, w2_row += _B01)
    {
        bufferType* tmp = lineStart;
        w0 = w0_row;
        w1 = w1_row;
        w2 = w2_row;
        /*bool lineDone(false);*/
        bool wasOk(false);
        for(int x = maxX - minX + 1; x /*&& !lineDone*/; --x, ++tmp, w0 += _A12, w1 += _A20, w2 += _A01)
        {
            if((w0|w1|w2) >= 0)
            {
                *tmp = color;
               wasOk = true;
            }
            else if(wasOk)
            {
                //lineDone = true;
                break;
            }
        }
    }
}
#else
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
    bufferType* lineStart = frameBuffer + minY * screenWidth + minX;
    for(int y = maxY - minY + 1; y; --y, lineStart += screenWidth, w1_row += _B23, w2_row += _B31, w3_row += _B12)
    {
        bufferType* tmp = lineStart;
        w1 = w1_row;
        w2 = w2_row;
        w3 = w3_row;
        /*bool lineDone(false);*/
        bool wasOk(false);
        for(int x = maxX - minX + 1; x /*&& !lineDone*/; --x, ++tmp, w1 += _A23, w2 += _A31, w3 += _A12)
        {
            if((w1|w2|w3) >= 0)
            {
                *tmp = triangle.color*100;//((x+y)&0x1 ? triangle.color*100 : 0xFFFFFF) ;
               wasOk = true;
            }
            else if(wasOk)
            {
                //lineDone = true;
                break;
            }
        }
    }
}

#endif

void grog::normalize(float* io) noexcept
{
  float norm =  (io[0] * io[0]) +
                (io[1] * io[1]) +
                (io[2] * io[2]);
  float invSqrt = 1.f/sqrtf(norm);
  io[0] *= invSqrt;
  io[1] *= invSqrt;
  io[2] *= invSqrt;
}

void grog::crossProd(float* left,
                     float* right,
                     float* out,
                     bool normalize) noexcept
{
  out[0] = left[1] * right[2] - right[1] * left[2];
  out[1] = left[2] * right[0] - right[2] * left[0];
  out[2] = left[0] * right[1] - right[0] * left[1];

  if(normalize)
  {
    grog::normalize(out);
  }
}
