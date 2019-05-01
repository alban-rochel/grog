#pragma once

#ifdef __linux

#include "grog.h"

#include <QPixmap>

namespace  grog
{
  class Display
  {
    public:

      Display() noexcept;
      ~Display() noexcept;

      void draw() noexcept;

      uint8_t* buffer;

      QPixmap* pixmap;
  };
}

#endif
