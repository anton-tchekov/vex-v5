/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       main.cpp                                                  */
/*    Author:       C:\Users\Anton                                            */
/*    Created:      Thu Jun 25 2020                                           */
/*    Description:  V5 project                                                */
/*                                                                            */
/*----------------------------------------------------------------------------*/

// ---- START VEXCODE CONFIGURED DEVICES ----
// ---- END VEXCODE CONFIGURED DEVICES ----

#include "vex.h"

using namespace vex;

void _vtriu(int x, int y, int w, int h)
{
  int step = 0;
  for(int i = y; step < h / 2; --i)
  {
    Brain.Screen.drawRectangle(x + step, i, w - 2 * step, 1);
    ++step;
  }
}

void _vtrid(int x, int y, int w, int h)
{
  int step = 0;
  for(int i = y; step < h / 2; ++i)
  {
    Brain.Screen.drawRectangle(x + step, i, w - 2 * step, 1);
    ++step;
  }
}

void _htrir(int x, int y, int w, int h)
{
  int step = 0;
  for(int i = x; step < w / 2; ++i)
  {
    Brain.Screen.drawRectangle(i, y + step, 1, h - 2 * step);
    ++step;
  }
}

void _htril(int x, int y, int w, int h)
{
  int step = 0;
  for(int i = x; step < w / 2; --i)
  {
    Brain.Screen.drawRectangle(i, y + step, 1, h - 2 * step);
    ++step;
  }
}

void _render_lcd(int x, int y, int w, int h, int bits, int old)
{
  int csize;
  csize = w / 6;

  /* Top */
  if((bits & (1 << 0)) != (old & (1 << 0)))
  {
    if(bits & (1 << 0))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x + csize, y, w - 2 * csize, csize);
    _htrir(x + w - csize, y, csize, csize);
    _htril(x + csize, y, csize, csize);
  }

  /* Right Top */
  if((bits & (1 << 1)) != (old & (1 << 1)))
  {
    if(bits & (1 << 1))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x + w - csize, y + csize, csize, h / 2 - 2 * csize + csize / 2);
    _vtriu(x + w - csize, y + csize, csize, csize);
    _vtrid(x + w - csize, y + csize + h / 2 - 2 * csize + csize / 2, csize, csize);
  }

  /* Right Bottom */
  if((bits & (1 << 2)) != (old & (1 << 2)))
  {
    if(bits & (1 << 2))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x + w - csize, y + h / 2 + csize / 2, csize, h / 2 - 2 * csize + csize / 2);
    _vtriu(x + w - csize, y + h / 2 + csize / 2, csize, csize);
    _vtrid(x + w - csize, y + h / 2 + csize / 2 + h / 2 - 2 * csize + csize / 2, csize, csize);
  }

  /* Bottom */
  if((bits & (1 << 3)) != (old & (1 << 3)))
  {
    if(bits & (1 << 3))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x + csize, y + h - csize, w - 2 * csize, csize);
    _htrir(x + w - csize, y + h - csize, csize, csize);
    _htril(x + csize, y + h - csize, csize, csize);
  }

  /* Left Bottom */
  if((bits & (1 << 4)) != (old & (1 << 4)))
  {
    if(bits & (1 << 4))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x, y + h / 2 + csize / 2, csize, h / 2 - 2 * csize + csize / 2);
    _vtriu(x, y + h / 2 + csize / 2, csize, csize);
    _vtrid(x, y + h / 2 + csize / 2 + h / 2 - 2 * csize + csize / 2, csize, csize);
  }

  /* Left Top */
  if((bits & (1 << 5)) != (old & (1 << 5)))
  {
    if(bits & (1 << 5))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x, y + csize, csize, h / 2 - 2 * csize + csize / 2);
    _vtriu(x, y + csize, csize, csize);
    _vtrid(x, y + csize + h / 2 - 2 * csize + csize / 2, csize, csize);
  }

  /* Center */
  if((bits & (1 << 6)) != (old & (1 << 6)))
  {
    if(bits & (1 << 6))
    {
      Brain.Screen.setPenColor(orange);
      Brain.Screen.setFillColor(orange);
    }
    else
    {
      Brain.Screen.setPenColor(black);
      Brain.Screen.setFillColor(black);
    }

    Brain.Screen.drawRectangle(x + csize, y + h / 2 - csize / 2, w - 2 * csize, csize);
    _htrir(x + w - csize, y + h / 2 - csize / 2, csize, csize);
    _htril(x + csize, y + h / 2 - csize / 2, csize, csize);
  }
}

void render_lcd(int x, int y, int w, int h, int number, int old)
{
  static int bits[11] =
  {
    0b0111111,
    0b0000110,
    0b1011011,
    0b1001111,
    0b1100110,

    0b1101101,
    0b1111101,
    0b0000111,
    0b1111111,
    0b1101111,

    0b0000000
  };

  _render_lcd(x, y, w, h, bits[number], bits[old]);
}

void render_info(int values[6], int old[6])
{
  static int first = 1;
  const int lMarginX = 10;
  const int lMarginY = 50;
  const int lWidth = 70;
  const int lHeight = 160;
  const int lSpacing = 10;
  const int cSpacing = 16;
  const int dSize = 10;
  const int sWidth = 50;
  const int sHeight = 100;
  int i, x;
  for(x = lMarginX, i = 0; i < 2; ++i)
  {
    render_lcd(x, lMarginY, lWidth, lHeight, values[i], old[i]);
    x += lSpacing + lWidth;
  }

  if(first)
  {
    Brain.Screen.setPenColor(orange);
    Brain.Screen.setFillColor(orange);    
    Brain.Screen.drawRectangle(x + cSpacing / 2 - lSpacing / 2 - dSize / 2, lMarginY + lHeight / 4 - dSize / 2, dSize, dSize);
    Brain.Screen.drawRectangle(x + cSpacing / 2 - lSpacing / 2 - dSize / 2, lMarginY + lHeight / 4 * 3 - dSize / 2, dSize, dSize);
  }

  for(x += cSpacing; i < 4; ++i)
  {
    render_lcd(x, lMarginY, lWidth, lHeight, values[i], old[i]);
    x += lSpacing + lWidth;
  }

  if(first)
  {
    Brain.Screen.setPenColor(orange);
    Brain.Screen.setFillColor(orange);
    Brain.Screen.drawRectangle(x + cSpacing / 2 - lSpacing / 2 - dSize / 2, lMarginY + lHeight - sHeight / 4 - dSize / 2, dSize, dSize);
    Brain.Screen.drawRectangle(x + cSpacing / 2 - lSpacing / 2 - dSize / 2, lMarginY + lHeight - sHeight / 4 * 3 - dSize / 2, dSize, dSize);
    first = 0;
  }

  for(x += cSpacing; i < 6; ++i)
  {
    render_lcd(x, lMarginY + lHeight - sHeight, sWidth, sHeight, values[i], old[i]);
    x += lSpacing + sWidth;
  }
}

int main()
{
  int numbers[6] = { 0 };
  int prev[6] = { 10, 10, 10, 10, 10, 10 };
  int sec = 0, hs = 0;

  vexcodeInit();
  
  vex::task::sleep(3000);

  Brain.Screen.clearScreen();
  Brain.Screen.setPenColor(orange);
  Brain.Screen.setFont(monoM);
  Brain.Screen.print("Active Time Remaining:       [ Autonomous Mode ]");

  render_info(numbers, prev);

  for(;;)
  {
    vex::task::sleep(10);
    memcpy(prev, numbers, 6 * sizeof(int));
    numbers[2] = sec / 10;
    numbers[3] = sec - (sec / 10) * 10;

    numbers[4] = hs / 10;
    numbers[5] = hs - (hs / 10) * 10;

    render_info(numbers, prev);
    ++hs;
    if(hs == 100)
    {
      hs = 0;
      ++sec;
    }
    
    if(hs == 1 && sec == 15)
    {
      break;
    }
  }
}

