#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro

#include <stdio.h>
#include <math.h>
#include <proc/p32mx170f256b.h>
#include "spi.h"
#include "ST7789.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = FRCPLL // use internal oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = OFF // internal RC
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0; // A4 output
    TRISBbits.TRISB4 = 1; // B4 input
    LATAbits.LATA4 = 0; // set A4 output to 0
    
    // enable interrupts
    __builtin_enable_interrupts();
    
    // Initial SPI communication + LCD Display
    initSPI();
    LCD_init();
    
    LCD_clearScreen(BLUE);
    
    char str[20];
    int i, j, bar_length = 100;
    float fps;
    
    while (1) {
        for (i = 0; i < bar_length; i++) {
            _CP0_SET_COUNT(0);
            // draw text
            sprintf(str, "Hello World %d", i);
            drawString(28, 32, WHITE, BLUE, str);
            
            // half of progress bar is green, the other half is green
            if (i < bar_length / 2) {
                drawProgressBar(28, 45, RED, WHITE, i, bar_length);
            } else {
                drawProgressBar(28, 45, GREEN, WHITE, i, bar_length);
            }
                        
            // draw fps
            fps = 24000000.00 / _CP0_GET_COUNT();
            sprintf(str, "FPS = %.3f", fps);
            drawString(28, 120, WHITE, BLUE, str);
        }
    }
}
