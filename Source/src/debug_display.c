/*
 * debug_display.c
 *
 *  Created on: Aug 23, 2014
 *      Author: nricc_000
 */

#include "debug_display.h"

long debug_display ()
{
  testFillScreen();
  testText();
  testLines(ILI9340_CYAN);
  testFastLines(ILI9340_RED, ILI9340_BLUE);
  testRects(ILI9340_GREEN);
  testFilledRects(ILI9340_YELLOW, ILI9340_MAGENTA);
  testFilledCircles(10, ILI9340_MAGENTA);
  testCircles(10, ILI9340_WHITE);
  testTriangles();
  testFilledTriangles();
  testRoundRects();
  testFilledRoundRects();
  return 300;
}

void testFillScreen() {
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  Adafruit_GFX_fillScreen(ILI9340_RED);
  Adafruit_GFX_fillScreen(ILI9340_GREEN);
  Adafruit_GFX_fillScreen(ILI9340_BLUE);
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
}

void testText() {
	Adafruit_GFX_fillScreen(ILI9340_BLACK);
	// FIXME: no println command
/*Adafruit_GFX_setCursor(0, 0);
Adafruit_GFX_setTextColor(ILI9341_WHITE); Adafruit_GFX_setTextSize(1);
Adafruit_GFX_println("Hello World!");
Adafruit_GFX_setTextColor(ILI9341_YELLOW); Adafruit_GFX_setTextSize(2);
Adafruit_GFX_println(1234.56);
Adafruit_GFX_setTextColor(ILI9341_RED); Adafruit_GFX_setTextSize(3);
Adafruit_GFX_println(0xDEADBEEF);
Adafruit_GFX_println();
Adafruit_GFX_setTextColor(ILI9341_GREEN);
Adafruit_GFX_setTextSize(5);
Adafruit_GFX_println("Groop");
Adafruit_GFX_setTextSize(2);
Adafruit_GFX_println("I implore thee,");
Adafruit_GFX_setTextSize(1);
Adafruit_GFX_println("my foonting turlingdromes.");
Adafruit_GFX_println("And hooptiously drangle me");
Adafruit_GFX_println("with crinkly bindlewurdles,");
Adafruit_GFX_println("Or I will rend thee");
Adafruit_GFX_println("in the gobberwarts");
Adafruit_GFX_println("with my blurglecruncheon,");
Adafruit_GFX_println("see if I don't!");*/
}
void testLines(int color) {
  int x1, y1, x2, y2,
  w = Adafruit_GFX_width(),
  h = Adafruit_GFX_height();
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  x1 = y1 = 0;
  y2 = h - 1;

  for(x2=0; x2<w; x2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
  x2 = w - 1;
  for(y2=0; y2<h; y2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;
  for(x2=0; x2<w; x2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
  x2 = 0;
  for(y2=0; y2<h; y2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  for(x2=0; x2<w; x2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
  x2 = w - 1;
  for(y2=0; y2<h; y2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);

  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  for(x2=0; x2<w; x2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
  x2 = 0;
  for(y2=0; y2<h; y2+=6) Adafruit_GFX_drawLine(x1, y1, x2, y2, color);
}
void testFastLines(int color1, int color2) {
  int x, y, w = Adafruit_GFX_width(), h = Adafruit_GFX_height();
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  for(y=0; y<h; y+=5) Adafruit_GFX_drawFastHLine(0, y, w, color1);
  for(x=0; x<w; x+=5) Adafruit_GFX_drawFastVLine(x, 0, h, color2);
}

void testRects(int color) {
  int n, i, i2,
  cx = Adafruit_GFX_width() / 2,
  cy = Adafruit_GFX_height() / 2;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  n = Adafruit_GFX_width() < Adafruit_GFX_height() ? Adafruit_GFX_width() : Adafruit_GFX_height();
  for(i=2; i<n; i+=6) {
    i2 = i / 2;
    Adafruit_GFX_drawRect(cx-i2, cy-i2, i, i, color);
  }
}

void testFilledRects(int color1, int color2) {
  int n, i, i2,
  cx = Adafruit_GFX_width() / 2 - 1,
  cy = Adafruit_GFX_height() / 2 - 1;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  n = Adafruit_GFX_width() < Adafruit_GFX_height() ? Adafruit_GFX_width() : Adafruit_GFX_height();
  for(i=n; i>0; i-=6) {
    i2 = i / 2;
    Adafruit_GFX_fillRect(cx-i2, cy-i2, i, i, color1);
    // Outlines are not included in timing results
    Adafruit_GFX_drawRect(cx-i2, cy-i2, i, i, color2);
  }
}

void testFilledCircles(int radius, int color) {
  int x, y, w = Adafruit_GFX_width(), h = Adafruit_GFX_height(), r2 = radius * 2;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  for(x=radius; x<w; x+=r2) {
    for(y=radius; y<h; y+=r2) {
    	Adafruit_GFX_fillCircle(x, y, radius, color);
    }
  }
}
void testCircles(int radius, int color) {
  int x, y, r2 = radius * 2,
  w = Adafruit_GFX_width() + radius,
  h = Adafruit_GFX_height() + radius;
  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  for(x=0; x<w; x+=r2) {
    for(y=0; y<h; y+=r2) {
    	Adafruit_GFX_drawCircle(x, y, radius, color);
    }
  }
}

void testTriangles() {
  int n, i, cx = Adafruit_GFX_width() / 2 - 1,
  cy = Adafruit_GFX_height() / 2 - 1;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  n = cx < cy ? cx : cy;
  for(i=0; i<n; i+=5) {
	Adafruit_GFX_drawTriangle(
    cx , cy - i, // peak
    cx - i, cy + i, // bottom left
    cx + i, cy + i, // bottom right
    Adafruit_ILI9340_Color565(0, 0, i));
  }
}
void testFilledTriangles() {
  int i, cx = Adafruit_GFX_width() / 2 - 1,
  cy = Adafruit_GFX_height() / 2 - 1;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  for(i=cx < cy ? cx : cy; i>10; i-=5) {
	Adafruit_GFX_fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
	Adafruit_ILI9340_Color565(0, i, i));
	Adafruit_GFX_drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
	Adafruit_ILI9340_Color565(i, i, 0));
  }
}

void testRoundRects() {
  int w, i, i2,
  cx = Adafruit_GFX_width() / 2 - 1,
  cy = Adafruit_GFX_height() / 2 - 1;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  w = Adafruit_GFX_width() < Adafruit_GFX_height() ? Adafruit_GFX_width() : Adafruit_GFX_height();
  for(i=0; i<w; i+=6) {
    i2 = i / 2;
    Adafruit_GFX_drawRoundRect(cx-i2, cy-i2, i, i, i/8, Adafruit_ILI9340_Color565(i, 0, 0));
  }
}
void testFilledRoundRects() {
  int w, i, i2,
  cx = Adafruit_GFX_width() / 2 - 1,
  cy = Adafruit_GFX_height() / 2 - 1;
  Adafruit_GFX_fillScreen(ILI9340_BLACK);
  w = Adafruit_GFX_width() < Adafruit_GFX_height() ? Adafruit_GFX_width() : Adafruit_GFX_height();
  for(i=w; i>20; i-=6) {
    i2 = i / 2;
    Adafruit_GFX_fillRoundRect(cx-i2, cy-i2, i, i, i/8, Adafruit_ILI9340_Color565(0, i, 0));
  }
}
