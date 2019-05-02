#include "Display.h"

#ifdef __linux
#include <QPainter>

using namespace grog;


namespace grog
{
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
  buffer(new uint8_t[DisplayWidth * DisplayHeight/2])
{
    memset(buffer, 0x0, DisplayWidth*DisplayHeight/2);
}

Display::~Display() noexcept
{
  delete[] buffer;
}

void Display::draw() noexcept
{
  if(!pixmap)
    return;

  QPainter painter(pixmap);

  uint8_t* currentBuffer = buffer;

  for(unsigned int y = 0; y < DisplayWidth; ++y)
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
  memset(buffer, 0x0, DisplayWidth*DisplayHeight/2);
}


#endif


