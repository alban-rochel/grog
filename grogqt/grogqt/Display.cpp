#include "Display.h"

#ifdef __linux
#include <QPainter>
#else
#include <Gamebuino-Meta.h>
#endif

using namespace grog;

#ifndef __linux
namespace Gamebuino_Meta
{
  extern volatile uint32_t dma_desc_free_count;
  static SPISettings tftSPISettings = SPISettings(24000000, MSBFIRST, SPI_MODE0);
}
#endif

namespace grog
{
  constexpr uint32_t ScreenWidth =  160;
  constexpr uint32_t ScreenHeight =  128;
  constexpr uint32_t StripHeight = 8;
  constexpr uint32_t StripSizePix = ScreenWidth * StripHeight;
  constexpr uint32_t StripSizeBytes = StripSizePix * 2;

  const uint16_t Palette[] = {  		/*black		*/  0x0000,
                                    /*darkblue */	0x0210,
                                    /*purple	*/	0x9008,
                                    /*green	  */	0x044A,
                                    /*brown		*/  0xCC68,
                                    /*darkgray*/	0x5268,
                                    /*gray		*/  0xACD0,
                                    /*white		*/  0xFFFF,
                                    /*red			*/  0xD8E4,
                                    /*orange	*/	0xFD42,
                                    /*yellow	 */ 0xF720,
                                    /*lightgreen*/0x8668,
                                    /*lightblue*/	0x7DDF,
                                    /*blue     */ 0x4439,
                                    /*pink		*/  0xCA30,
                                    /*beige		*/  0xFEB2,
                             };
}


Display::Display() noexcept:
  buffer(new uint8_t[ScreenWidth * ScreenHeight / 8]),
  strip1(new uint16_t[StripSizePix]),
  strip2(new uint16_t[StripSizePix])
{
#ifdef __linux
    memset(buffer, 0x0, 80*64/2);
#endif
}

Display::~Display() noexcept
{
  delete[] buffer;
  delete[] strip1;
  delete[] strip2;
}

void Display::draw() noexcept
{
  QPainter painter(pixmap);

  uint8_t* currentBuffer = buffer;

  for(unsigned int y = 0; y < 64; ++y)
  {
      for(unsigned int x = 0; x < 80; x+=2)
      {
        uint8_t colors = (*currentBuffer++);
        uint16_t col = grog::Palette[colors & 0x0F];
        uint8_t red = (uint8_t)((col & 0xF800) >> 8);
        uint8_t green = (uint8_t)((col & 0x07E0) >> 3);
        uint8_t blue = (uint8_t)((col & 0x001F) << 3);
        painter.setPen(QColor::fromRgb(red, green, blue));
        painter.drawPoint(x+1, y);

        col = grog::Palette[colors >> 4];
        red = (uint8_t)((col & 0xF800) >> 8);
        green = (uint8_t)((col & 0x07E0) >> 3);
        blue = (uint8_t)((col & 0x001F) << 3);
        painter.setPen(QColor::fromRgb(red, green, blue));
        painter.drawPoint(x, y);
      }
  }
  memset(buffer, 0x0, 80*64/2);
}

uint16_t* Display::startFrame() noexcept
{
    return nullptr;
}

void Display::endFrame() noexcept
{
}

uint16_t* Display::commitStrip(/*GraphicsManager::TaskSet* taskSet*/) noexcept
{
  return nullptr;
}


