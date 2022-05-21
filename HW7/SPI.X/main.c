#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include <stdio.h>
#include <math.h>
#include <proc/p32mx170f256b.h>
#include "spi.h"

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

// CONSTANTS
#define VMAX 1023           // 2^10 - 1 since DAC is 10-bit. 3.3V

#ifndef M_PI                // should be defined in math.h
#define M_PI 3.141592653
#endif

unsigned short genSineWave(unsigned int i, float dt);
unsigned short genTriangleWave(unsigned int i, float dt, int mid);
void writeToDAC(unsigned char ab, unsigned short v);

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
    
    // Initial SPI communication
    initSPI();

    // enable interrupts
    __builtin_enable_interrupts();
    
    int points = 1024;
    int mid = points / 2;
    float dt = 1.0/points;
    
    // since the waves are repeating, we don't need to keep recalculating them 
    // after one period
    float sinWave[points];
    float triWave[points];
    
    int i;
    for (i = 0; i < points; i++) {
        sinWave[i] = genSineWave(i, dt);
        triWave[i] = genTriangleWave(i, dt, mid);
    }
    
    while (1) {
        
        for (i = 0; i < points; i++) {
            writeToDAC(0, sinWave[i]);
            writeToDAC(1, triWave[i]);
        }
        
        // Blink the LED every 1 second
        LATAbits.LATA4 = !LATAbits.LATA4;
                
        _CP0_SET_COUNT(0);
        
        while (_CP0_GET_COUNT() < 24000000 * dt); // 24MHz core clock
    }
}

void writeToDAC(unsigned char ab, unsigned short v) {
    unsigned short p;
    p = ab << 15;
    p = p | (0b111 << 12);
    p = p | (v << 2);
    
    // write the 16 bits over SDO1
    LATAbits.LATA0 = 0; // CS low
    
    spi_io(p>>8);       // write first byte
    spi_io(p);          // write second byte (will truncate most significant bits)
    
    LATAbits.LATA0 = 1;  // CS high
}

unsigned short genSineWave(unsigned int i, float dt) {
    return (VMAX/2.0) + (VMAX/2.0) * sin(6.0 * M_PI * i * dt);
}

unsigned short genTriangleWave(unsigned int i, float dt, int mid) {
    float res;
    
    if (i < mid) {
        res = VMAX * 2 * i * dt;
    } else {
        res = (VMAX*2) - (VMAX * 2 * i * dt);
    }
    return res;
}
