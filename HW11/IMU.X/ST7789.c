// functions to operate the ST7789 on the PIC32
// adapted from https://github.com/sumotoy/TFT_ST7789
// and https://github.com/adafruit/Adafruit-ST7789-Library

// pin connections:
// GND - GND
// VCC - 3.3V
// SCL - B14
// SDA - B13
// RES - B15
// DC - B12
// BLK - NC

#include <xc.h>
#include "ST7789.h"
#include "spi.h"
#include "font.h"
#include "math.h"

void LCD_command(unsigned char com) {
    LATBbits.LATB12 = 0; // DC
    spi_io(com);
}

void LCD_data(unsigned char dat) {
    LATBbits.LATB12 = 1; // DC
    spi_io(dat);
}

void LCD_data16(unsigned short dat) {
    LATBbits.LATB12 = 1; // DC
    spi_io(dat>>8);
    spi_io(dat);
}

void LCD_init() {
  unsigned int time = 0;
  LCD_command(ST7789_SWRESET); //software reset
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.15) {}
  
  LCD_command(ST7789_SLPOUT); //exit sleep
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.5) {}
  
  LCD_command(ST7789_COLMOD);
  LCD_data(0x55);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.01) {}
  
  LCD_command(ST7789_MADCTL);
  LCD_data(0x00);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.001) {}
  
  LCD_command(ST7789_CASET);
  LCD_data(0x00);
  LCD_data(ST7789_XSTART);
  LCD_data((240+ST7789_XSTART)>>8);
  LCD_data((240+ST7789_XSTART)&0xFF);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.001) {}

  LCD_command(ST7789_RASET);
  LCD_data(0x00);
  LCD_data(ST7789_YSTART);
  LCD_data((240+ST7789_YSTART)>>8);
  LCD_data((240+ST7789_YSTART)&0xFF);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.001) {}
  
  LCD_command(ST7789_INVON);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.01) {}

  LCD_command(ST7789_NORON);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.01) {}
  
  LCD_command(ST7789_DISPON);
  time = _CP0_GET_COUNT();
  while (_CP0_GET_COUNT() < time + 48000000/2*0.5) {}
}

void LCD_drawPixel(unsigned short x, unsigned short y, unsigned short color) {
  // should check boundary first
  LCD_setAddr(x,y,x+1,y+1);
  LCD_data16(color);
}

void LCD_setAddr(unsigned short x0, unsigned short y0, unsigned short x1, unsigned short y1) {
  LCD_command(ST7789_CASET); // Column
  LCD_data16(x0+ST7789_XSTART);
  LCD_data16(x1+ST7789_XSTART);
  
  LCD_command(ST7789_RASET); // Page
  LCD_data16(y0+ST7789_YSTART);
  LCD_data16(y1+ST7789_YSTART);

  LCD_command(ST7789_RAMWR); // Into RAM
}

void LCD_clearScreen(unsigned short color) {
  int i;
  LCD_setAddr(0,0,_GRAMWIDTH,_GRAMHEIGH);
	for (i = 0;i < _GRAMSIZE; i++){
		LCD_data16(color);
	}
}

// drawChar function
void drawChar(unsigned short x, unsigned short y, unsigned short color, unsigned short background, unsigned char c) {
    int xo, yo;
    unsigned char bits;
       
    for (xo = 0; xo < 5; xo++) {
        bits = ASCII[c-0x20][xo];
        for (yo = 0; yo < 8; yo++) {
            if ((bits >> yo) & 1) {
                LCD_drawPixel(x+xo, y+yo, color);
            } else {
                LCD_drawPixel(x+xo, y+yo, background);
            }
        }
    }
}

// drawString function
void drawString(unsigned short x, unsigned short y, unsigned short color, unsigned short background, char* str) {
    int i = 0;
    unsigned char c;
    while (str[i]) {
        c = str[i];
        drawChar(x + (i*6), y, color, background, c);
        i++;
    }
}

void drawProgressBar(unsigned short x, unsigned short y, unsigned short color, unsigned short background, int progress, int length) {
    int i,j,h = 4;
    
    
    // draw progress in indicated color
    for (i = 0; i < progress; i++) {
        for (j = 0; j < h; j++) {
            LCD_drawPixel(x+i, y+j, color);
        }
    }
    
    // draw the rest of the background
    for ( ; i < length; i++) {
        for (j = 0; j < h; j++) {
            LCD_drawPixel(x+i, y+j, background);
        }
    }
}

void drawBlock(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2, unsigned short color) {
    int x, y;
    for (x = x1; x < x2; x++) {
        for (y = y1; y < y2; y++) {
            LCD_drawPixel(x, y, color);
        }
    }
}

// x and y are the center
void drawXBar(unsigned short x, unsigned short y, unsigned short color, unsigned short background, signed short progX) {
    signed short val = abs(progX * 60 / 16384);
    int i, j, width = 5;
    
    if (progX < 0) { // down
        for (i=0; i < val; i++) {
            for (j=0; j < width; j++) {
                LCD_drawPixel(x+j, y+i, color);
            }
        }
    } else { // up
        for (i=0; i < val; i++) {
            for (j=0; j < width; j++) {
                LCD_drawPixel(x+j, y-i, color);
            }
        }
    }
}

void drawYBar(unsigned short x, unsigned short y, unsigned short color, unsigned short background, signed short progY) {
    signed short val = abs(progY * 60 / 16384);
    int i, j, width = 5;
    
    if (progY < 0) { // right
        for (i=0; i < val; i++) {
            for (j=0; j < width; j++) {
                LCD_drawPixel(x+i, y+j, color);
            }
        }
    } else { // left
        for (i=0; i < val; i++) {
            for (j=0; j < width; j++) {
                LCD_drawPixel(x-i, y+j, color);
            }
        }
    }
}