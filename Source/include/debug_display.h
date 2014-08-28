#ifndef DEBUG_DISPLAY_H_
#define DEBUG_DISPLAY_H_

#include "Adafruit_ILI9340.h"

long debug_display();
void testFillScreen();
void testText();
void testLines(int color);
void testFastLines(int color1, int color2);
void testRects(int color);
void testFilledRects(int color1, int color2);
void testFilledCircles(int radius, int color);
void testCircles(int radius, int color);
void testRoundRects();
void testFilledRoundRects();

#endif
