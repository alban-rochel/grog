#include "Triangle.h"
#include "grog.h"
#include "Math.h"

using namespace grog;

namespace grog
{
  GROG_INLINE const int32_t& Min2(const int32_t &t1, const int32_t &t2) noexcept
  {
      return (t1 < t2 ? t1 : t2);
  }

  GROG_INLINE const int32_t& Max2(const int32_t &t1, const int32_t &t2) noexcept
  {
      return (t1 > t2 ? t1 : t2);
  }

  GROG_INLINE const int32_t& Min3(const int32_t& t1, const int32_t &t2, const int32_t &t3) noexcept
  {
      return (t1 < t2 ? Min2(t1, t3) : Min2(t2, t3));
  }

  GROG_INLINE const int32_t& Max3(const int32_t &t1, const int32_t &t2, const int32_t &t3) noexcept
  {
      return (t1 > t2 ? Max2(t1, t3) : Max2(t2, t3));
  }

}

void Triangle::rasterize(uint8_t *frameBuffer) const noexcept
{
  // Compute bounding boxes

  int32_t minX = Min3(p1x, p2x, p3x);
  int32_t maxX = Max3(p1x, p2x, p3x);
  int32_t minY = Min3(p1y, p2y, p3y);
  int32_t maxY = Max3(p1y, p2y, p3y);

  // Clip against screen bounds;

  minX = Max2(minX, 0);
  minY = Max2(minY, 0);
  maxX = Min2(maxX, DisplayWidth - 1);
  maxY = Min2(maxY, DisplayHeight - 1);

  // Rasterize

  // Triangle setup
  int _A12 = p1y - p2y, _B12 = p2x - p1x;
  int _A23 = p2y - p3y, _B23 = p3x - p2x;
  int _A31 = p3y - p1y, _B31 = p1x - p3x;

  int w1_row = Math::Orient2d(p2x, p2y, p3x, p3y, minX, minY);
  int w2_row = Math::Orient2d(p3x, p3y, p1x, p1y, minX, minY);
  int w3_row = Math::Orient2d(p1x, p1y, p2x, p2y, minX, minY);

  int w1(0), w2(0), w3(0);
  uint8_t* lineStart = frameBuffer + (minY * DisplayWidth + minX) / 2;
  bool upperNibbleStart = (minY * DisplayWidth + minX) & 0x1;
  const uint8_t& colorLower = (uint8_t)(color) & 0x0F;
  uint8_t colorUpper = colorLower << 4;

  for(int y = maxY - minY + 1; y; --y, lineStart += DisplayWidth/2, w1_row += _B23, w2_row += _B31, w3_row += _B12)
  {
    uint8_t* tmp = lineStart;
    bool upperNibble = upperNibbleStart;

    w1 = w1_row;
    w2 = w2_row;
    w3 = w3_row;

    bool wasOk(false);
    for(int x = maxX - minX + 1; GROG_LIKELY(x); --x, w1 += _A23, w2 += _A31, w3 += _A12)
    {
      if((w1|w2|w3) >= 0)
      {
        if(upperNibble)
        {
          *tmp = (*tmp & 0xF0) | colorLower;
          ++tmp;
        }
        else
        {
          *tmp = (*tmp & 0x0F) | colorUpper;
        }
        wasOk = true;
      }
      else
      {
        if(wasOk)
        {
          break;
        }
        if(upperNibble)
        {
          ++tmp;
        }
      }

      upperNibble = ! upperNibble;

    }
  }
}
