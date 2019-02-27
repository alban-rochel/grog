#pragma once

#include <cstdint>

namespace grog
{
  const uint16_t Palette[] = {  0x0000, // black
                                0xACD0, // gray
                                0x5268, // dark gray
                                0xFFFF, // white
                                0x9008, // purple
                                0xCA30, // magenta
                                0xD8E4, // red
                                0xFD42, // orange
                                0xCC68, // brown
                                0xFEB2, // beige
                                0xF720, // yellow
                                0x8668, // light green
                                0x044A, // green
                                0x0210, // dark blue
                                0x4439, // blue
                                0x7DDF // light blue
                             };

  enum class Color: uint8_t
  {
    Black       = 0,
    Gray        = 1,
    DarkGray    = 2,
    White       = 3,
    Purple      = 4,
    Magenta     = 5,
    Red         = 6,
    Orange      = 7,
    Brown       = 8,
    Beige       = 9,
    Yellow      = 10,
    LightGreen  = 11,
    Green       = 12,
    DarkBlue    = 13,
    Blue        = 14,
    LightBlue   = 15,
  };

  constexpr uint8_t color(Color col1, Color col2) noexcept
  {
    return  static_cast<uint8_t>(col1) |
            (static_cast<uint8_t>(col2) << 4);
  }

  constexpr uint8_t color(Color col) noexcept
  {
    return color(col, col);
  }



}
