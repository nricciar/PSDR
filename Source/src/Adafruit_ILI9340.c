/***************************************************
  This is an Arduino Library for the Adafruit 2.2" SPI display.
  This library works with the Adafruit 2.2" TFT Breakout w/SD card
  ----> http://www.adafruit.com/products/1480

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

#include "Adafruit_ILI9340.h"

#include <spi.h>
#include <hal.h>
#include <stdbool.h>

#include "stm32f4xx_hal_spi.h"

#if defined(__SAM3X8E__)
#include <include/pio.h>
  #define SET_BIT(port, bitMask) (port)->PIO_SODR |= (bitMask)
  #define CLEAR_BIT(port, bitMask) (port)->PIO_CODR |= (bitMask)
  #define USE_SPI_LIBRARY
#endif
#ifdef __AVR__
  #define SET_BIT(port, bitMask) *(port) |= (bitMask)
  #define CLEAR_BIT(port, bitMask) *(port) &= ~(bitMask)
#endif
#if defined(__arm__) && defined(CORE_TEENSY)
  #define USE_SPI_LIBRARY
  #define SET_BIT(port, bitMask) digitalWrite(*(port), HIGH);
  #define CLEAR_BIT(port, bitMask) digitalWrite(*(port), LOW);
#endif

//   int  _width = ILI9340_TFTWIDTH; //240;
//   int  _height = ILI9340_TFTHEIGHT; //320
	uint8_t rxBuf[10];
	uint8_t txBuf[2];

// Constructor when using hardware SPI.  Faster, but must use SPI pins
// specific to each board type (e.g. 11,13 for Uno, 51,52 for Mega, etc.)
void Adafruit_ILI9340_Adafruit_ILI9340(uint8_t cs, uint8_t dc, uint8_t rst) /*: Adafruit_GFX(ILI9340_TFTWIDTH, ILI9340_TFTHEIGHT) */ {
  hwSPI = true;
}

void Adafruit_ILI9340_spiwrite(uint8_t c) {
  if (hwSPI) {
#ifdef __AVR__
    SPDR = c;
    while(!(SPSR & _BV(SPIF)));
#endif
#if defined(USE_SPI_LIBRARY)
    SPI.transfer(c);
#endif
  } else {
    // Fast SPI bitbang swiped from LPD8806 library
	  uint8_t bit;
    for(bit = 0x80; bit; bit >>= 1) {
      if(c & bit) {
    	  HAL_GPIO_WritePin(SPI1_MOSI.port, SPI1_MOSI.pin, 1);
      } else {
    	  HAL_GPIO_WritePin(SPI1_MOSI.port, SPI1_MOSI.pin, 0);
      }
      HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 1);
      HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 0);
    }
  }
}


void Adafruit_ILI9340_writecommand(uint8_t c) {
	HAL_GPIO_WritePin(LCD_DC.port, LCD_DC.pin, 0);
	HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 0);
	HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);

	txBuf[0] = c;
	spi_readWrite(SpiHandle, rxBuf, txBuf, 1);

	HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);
}


void Adafruit_ILI9340_writedata(uint8_t c) {
	HAL_GPIO_WritePin(LCD_DC.port, LCD_DC.pin, 1);
	HAL_GPIO_WritePin(SPI1_SCK.port, SPI1_SCK.pin, 0);
	HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);

	txBuf[0] = c;
	spi_readWrite(SpiHandle, rxBuf, txBuf, 1);

	HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);
}

#define DELAY 0x80

static const uint8_t init_commands[] = {
  4, 0xEF, 0x03, 0x80, 0x02,
  4, 0xCF, 0x00, 0XC1, 0X30,
  5, 0xED, 0x64, 0x03, 0X12, 0X81,
  4, 0xE8, 0x85, 0x00, 0x78,
  6, 0xCB, 0x39, 0x2C, 0x00, 0x34, 0x02,
  2, 0xF7, 0x20,
  3, 0xEA, 0x00, 0x00,
  2, ILI9340_PWCTR1, 0x23, // Power control
  2, ILI9340_PWCTR2, 0x10, // Power control
  3, ILI9340_VMCTR1, 0x3e, 0x28, // VCM control
  2, ILI9340_VMCTR2, 0x86, // VCM control2
  2, ILI9340_MADCTL, 0x48, // Memory Access Control
  2, ILI9340_PIXFMT, 0x55,
  3, ILI9340_FRMCTR1, 0x00, 0x18,
  4, ILI9340_DFUNCTR, 0x08, 0x82, 0x27, // Display Function Control
  2, 0xF2, 0x00, // Gamma Function Disable
  2, ILI9340_GAMMASET, 0x01, // Gamma curve selected
  16, ILI9340_GMCTRP1, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08,
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00, // Set Gamma
  16, ILI9340_GMCTRN1, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07,
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F, // Set Gamma
  0
};

void Adafruit_ILI9340_begin(void) {
  HAL_GPIO_WritePin(LCD_RESET.port, LCD_RESET.pin, 1);
  hal_delay_ms(5);

  HAL_GPIO_WritePin(LCD_RESET.port, LCD_RESET.pin, 0);
  hal_delay_ms(20);

  HAL_GPIO_WritePin(LCD_RESET.port, LCD_RESET.pin, 1);
  hal_delay_ms(150);

  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0); //I'm going to try leaving this low the WHOLE TIME!

  const uint8_t *addr = init_commands;
  while (1) {
    uint8_t count = *addr++;
    if (count-- == 0) break;
    Adafruit_ILI9340_writecommand(*addr++);
    while (count-- > 0) {
    	Adafruit_ILI9340_writedata(*addr++);
    }
  }

  Adafruit_ILI9340_writecommand(ILI9340_SLPOUT);    //Exit Sleep
  hal_delay_ms(120);
  Adafruit_ILI9340_writecommand(ILI9340_DISPON);    //Display on
}


void Adafruit_ILI9340_setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1,
 uint16_t y1) {

	Adafruit_ILI9340_writecommand(ILI9340_CASET); // Column addr set
	Adafruit_ILI9340_writedata(x0 >> 8);
	Adafruit_ILI9340_writedata(x0 & 0xFF);     // XSTART
	Adafruit_ILI9340_writedata(x1 >> 8);
	Adafruit_ILI9340_writedata(x1 & 0xFF);     // XEND

	Adafruit_ILI9340_writecommand(ILI9340_PASET); // Row addr set
	Adafruit_ILI9340_writedata(y0>>8);
	Adafruit_ILI9340_writedata(y0);     // YSTART
	Adafruit_ILI9340_writedata(y1>>8);
	Adafruit_ILI9340_writedata(y1);     // YEND

	Adafruit_ILI9340_writecommand(ILI9340_RAMWR); // write to RAM
}


void Adafruit_ILI9340_pushColor(uint16_t color) {
	GPIO_WriteBit(LCD_DC.port, LCD_DC.pin, 1);
	GPIO_WriteBit(LCD_NSS.port, LCD_NSS.pin, 0);

	uint8_t rxBuf[10];
	uint8_t txBuf[2] = {color >> 8, color};
	spi_readWrite(SpiHandle, rxBuf, txBuf, 2);

	GPIO_WriteBit(LCD_NSS.port, LCD_NSS.pin, 1);
}

void Adafruit_ILI9340_drawPixel(int16_t x, int16_t y, uint16_t color) {

  if((x < 0) ||(x >= _width) || (y < 0) || (y >= _height)) return;

  Adafruit_ILI9340_setAddrWindow(x,y,x+1,y+1);

  HAL_GPIO_WritePin(LCD_DC.port, LCD_DC.pin, 1);
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);

  uint8_t rxBuf[10];
  uint8_t txBuf[2] = {color >> 8, color};
  spi_readWrite(SpiHandle, rxBuf, txBuf, 2);

  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);
}


void Adafruit_ILI9340_drawFastVLine(int16_t x, int16_t y, int16_t h,
 uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;

  if((y+h-1) >= _height)
    h = _height-y;

  Adafruit_ILI9340_setAddrWindow(x, y, x, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  HAL_GPIO_WritePin(LCD_DC.port, LCD_DC.pin, 1);
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);

  uint8_t rxBuf[10];
  uint8_t txBuf[2];
  while (h--) {
	txBuf[0] = hi;
	txBuf[1] = lo;

	spi_readWrite(SpiHandle, rxBuf, txBuf, 2);
  }
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);
}


void Adafruit_ILI9340_drawFastHLine(int16_t x, int16_t y, int16_t w,
  uint16_t color) {

  // Rudimentary clipping
  if((x >= _width) || (y >= _height)) return;
  if((x+w-1) >= _width)  w = _width-x;
  Adafruit_ILI9340_setAddrWindow(x, y, x+w-1, y);

  uint8_t hi = color >> 8, lo = color;
  HAL_GPIO_WritePin(LCD_DC.port, LCD_DC.pin, 1);
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);

  while (w--) {
	txBuf[0] = hi;
	txBuf[1] = lo;

	spi_readWrite(SpiHandle, rxBuf, txBuf, 2);
  }
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);
}

void Adafruit_ILI9340_fillScreen(uint16_t color) {
	Adafruit_ILI9340_fillRect(0, 0,  _width, _height, color);
}

// fill a rectangle
void Adafruit_ILI9340_fillRect(int16_t x, int16_t y, int16_t w, int16_t h,
  uint16_t color) {

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= _width) || (y >= _height)) return;
  if((x + w - 1) >= _width)  w = _width  - x;
  if((y + h - 1) >= _height) h = _height - y;

  Adafruit_ILI9340_setAddrWindow(x, y, x+w-1, y+h-1);

  uint8_t hi = color >> 8, lo = color;

  HAL_GPIO_WritePin(LCD_DC.port, LCD_DC.pin, 1);
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 0);

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
  	  txBuf[0] = hi;
  	  txBuf[1] = lo;

  	  spi_readWrite(SpiHandle, rxBuf, txBuf, 2);
    }
  }
  HAL_GPIO_WritePin(LCD_NSS.port, LCD_NSS.pin, 1);
}


// Pass 8-bit (each) R,G,B, get back 16-bit packed color
uint16_t Adafruit_ILI9340_Color565(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}


void Adafruit_ILI9340_setVertialScrollStartAddress(uint16_t address)
{
	Adafruit_ILI9340_writecommand(ILI9340_VSCRSADD);
	Adafruit_ILI9340_writedata((address >> 8) & 0xFF);
	Adafruit_ILI9340_writedata(address & 0xFF);
}

void Adafruit_ILI9340_setVerticalScrollDefinition(uint16_t topFixedArea,
		uint16_t verticalScrollArea, uint16_t bottomFixedArea)
{
	if (topFixedArea + verticalScrollArea + bottomFixedArea != 320) return;

	Adafruit_ILI9340_writecommand(ILI9340_VSCRDEF);
	Adafruit_ILI9340_writedata((topFixedArea >> 8) & 0xFF);
	Adafruit_ILI9340_writedata(topFixedArea & 0xFF);
	Adafruit_ILI9340_writedata((verticalScrollArea >> 8) & 0xFF);
	Adafruit_ILI9340_writedata(verticalScrollArea & 0xFF);
	Adafruit_ILI9340_writedata((bottomFixedArea >> 8) & 0xFF);
	Adafruit_ILI9340_writedata(bottomFixedArea & 0xFF);
}

void Adafruit_ILI9340_setRotation(uint8_t m) {

	Adafruit_ILI9340_writecommand(ILI9340_MADCTL);
  int rotation = m % 4; // can't be higher than 3
  switch (rotation) {
   case 0:
	   Adafruit_ILI9340_writedata(ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
     break;
   case 1:
	   Adafruit_ILI9340_writedata(ILI9340_MADCTL_MV | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  case 2:
	  Adafruit_ILI9340_writedata(ILI9340_MADCTL_MY | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTWIDTH;
     _height = ILI9340_TFTHEIGHT;
    break;
   case 3:
	   Adafruit_ILI9340_writedata(ILI9340_MADCTL_MV | ILI9340_MADCTL_MY | ILI9340_MADCTL_MX | ILI9340_MADCTL_BGR);
     _width  = ILI9340_TFTHEIGHT;
     _height = ILI9340_TFTWIDTH;
     break;
  }
}


void Adafruit_ILI9340_invertDisplay(bool i) {
	Adafruit_ILI9340_writecommand(i ? ILI9340_INVON : ILI9340_INVOFF);
}
