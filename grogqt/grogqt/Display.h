#pragma once

#include "grog.h"

#ifdef __linux
#include <QPixmap>
#endif

namespace  grog
{
  class Display
  {
    public:

      Display() noexcept;
      ~Display() noexcept;

      void draw() noexcept;

      uint8_t* buffer;

#ifdef __linux
      QPixmap* pixmap;
#endif

    private:
          // returns the buffer for the next strip
      uint16_t* startFrame() noexcept;

      void endFrame() noexcept;

      // returns the buffer for the next strip
      uint16_t* commitStrip() noexcept;

     uint16_t* strip1;
     uint16_t* strip2;
     uint16_t* currentStrip;
     uint16_t* sentStrip;
  };
}
