#include "Display.h"
#include "Colors.h"

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

#ifndef __linux
  constexpr uint32_t DmaDescCount = 3;

  static void WaitForDescAvailable(const uint32_t min_desc_num/*, GraphicsManager::TaskSet* taskSet*/) noexcept
  {
    while (Gamebuino_Meta::dma_desc_free_count < min_desc_num)
    {
      /*if(taskSet && taskSet->currentTask < taskSet->taskCount) // perform short tasks instead of losing CPU cycles
      {
        (*(taskSet->tasks[taskSet->currentTask].function))(taskSet->tasks[taskSet->currentTask].param);
          ++taskSet->currentTask;
      }*/
    }
  }


  static void WaitForTransfersDone() noexcept
  {
    WaitForDescAvailable(DmaDescCount/*, nullptr*/);
  }
#endif
}


Display::Display() noexcept:
  buffer(new uint8_t[ScreenWidth * ScreenHeight / 8]),
  strip1(new uint16_t[StripSizePix]),
  strip2(new uint16_t[StripSizePix])
{}

Display::~Display() noexcept
{
  delete[] buffer;
  delete[] strip1;
  delete[] strip2;
}

#ifdef __linux

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
        uint8_t red = (uint8_t)(col & 0xF8);
        uint8_t green = (uint8_t)(((col & 0xE000) >> 13)| ((col & 7) << 5));
        uint8_t blue = (uint8_t)((col & 0x1F00) >> 5);
        painter.setPen(QColor::fromRgb(red, green, blue));
        painter.drawPoint(x, y);

        col = grog::Palette[colors >> 4];
        red = (uint8_t)(col & 0xF8);
        green = (uint8_t)(((col & 0xE000) >> 13)| ((col & 7) << 5));
        blue = (uint8_t)((col & 0x1F00) >> 5);
        painter.setPen(QColor::fromRgb(red, green, blue));
        painter.drawPoint(x+1, y);
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

#else

void Display::draw() noexcept
{
  uint16_t* strip = startFrame();
  uint16_t* stripCursor = strip;
  uint16_t* stripCursorNextLine = stripCursor + ScreenWidth;

  uint8_t* currentBuffer = buffer;

  uint32_t stripLine = 0;
  for(uint32_t screenLine = 0; screenLine < ScreenHeight; screenLine += 2, stripLine += 2)
  {
    if(stripLine == StripHeight)
    {
      strip = commitStrip();
      stripCursor = strip;
      stripCursorNextLine = stripCursor + ScreenWidth;
      stripLine = 0;
    }

      for(unsigned int x = 0; x < 80; x+=2)
      {
        uint8_t colors = (*currentBuffer++);
        uint16_t col = grog::Palette[colors & 0x0F];
        (*stripCursor++) = col;
        (*stripCursor++) = col;
        (*stripCursorNextLine++) = col;
        (*stripCursorNextLine++) = col;
        
        col = grog::Palette[colors >> 4];
        (*stripCursor++) = col;
        (*stripCursor++) = col;
        (*stripCursorNextLine++) = col;
        (*stripCursorNextLine++) = col;
      }
      stripCursor += ScreenWidth;
      stripCursorNextLine += ScreenWidth;



  }
  commitStrip();

  endFrame();

  memset(buffer, 0x0, 80*64/2);
}

uint16_t* Display::startFrame() noexcept
{
    gb.tft.setAddrWindow(0, 0, ScreenWidth - 1, ScreenHeight - 1);
    //initiate SPI
    SPI.beginTransaction(Gamebuino_Meta::tftSPISettings);
    gb.tft.dataMode();

    currentStrip = strip1;
    sentStrip = strip2;

    return currentStrip;
}

void Display::endFrame() noexcept
{
  WaitForTransfersDone();
  gb.tft.idleMode();
  SPI.endTransaction();

}

uint16_t* Display::commitStrip(/*GraphicsManager::TaskSet* taskSet*/) noexcept
{
    uint16_t *temp = currentStrip;
    currentStrip = sentStrip;
    sentStrip = temp;

    gb.tft.sendBuffer(sentStrip, StripSizePix);

    WaitForDescAvailable(2/*, taskSet*/);

    return currentStrip;
}
#endif
