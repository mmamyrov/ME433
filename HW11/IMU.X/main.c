#include <xc.h>           // processor SFR definitions
#include <sys/attribs.h>  // __ISR macro

#include <stdio.h>
#include <math.h>
#include <proc/p32mx170f256b.h>
#include <cp0defs.h>
#include "spi.h"
#include "ST7789.h"
#include "imu.h"

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
    imu_setup();
    
    LCD_clearScreen(BLACK);
    
    int data_len = 7;
    char str[100];
    signed short data[data_len];
    
    float fps;
    
    while (1) {
        
        _CP0_SET_COUNT(0);
        imu_read(data, data_len);
        
        // Print the text data on display
        sprintf(str, "GYRO: %6d  %6d  %6d", data[1], data[2], data[3]);
        drawString(28, 32, WHITE, BLACK, str);
        sprintf(str, "ACCEL: %6d  %6d  %6d", data[4], data[5], data[6]);
        drawString(28, 54, WHITE, BLACK, str);
        sprintf(str, "TEMP: %6d", data[0]);
        drawString(28, 72, WHITE, BLACK, str);
        
        drawBlock(120, 88, 125, 228, BLACK);    // remove X bar
        drawBlock(40, 148, 200, 153, BLACK);     // remove Y bar
        
        // draw the inclinometer bars
        drawXBar(120, 148, MAGENTA, BLACK, data[4]);
        drawYBar(120, 148, RED, BLACK, data[5]);
        
        fps = 24000000.0 / _CP0_GET_COUNT();
        sprintf(str, "FPS = %.3f", fps);
        drawString(28, 120, WHITE, BLACK, str);
    }
}
