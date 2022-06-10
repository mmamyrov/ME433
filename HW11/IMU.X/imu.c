#include "imu.h"
#include "i2c_master_noint.h"

void imu_setup(void) {
    i2c_master_setup();
    
    unsigned char who = readPin(IMU_WADDR, WHO_AM_I);
    
    if (who != WHO_AM_I_VAL) {
        while(1);
    }
    
    setPin(IMU_WADDR, CTRL1_XL, 0b10000010);    // accel 1.66 kHz, 2g sens, 100 Hz filter
    setPin(IMU_WADDR, CTRL2_G, 0b10001000);     // gyro 1.66 kHz, 1000 dps
    setPin(IMU_WADDR, CTRL3_C, 0b00000100);     // enable IF_INC
}

void imu_read(signed short* data, int length) {
    int char_len = length * 2;
    unsigned char char_data[char_len];
    
    i2c_read_multiple(IMU_WADDR, OUT_TEMP_L, char_data, char_len);
    
    int i;
    int i2;
    
    for (i=0; i < length; i++) {
        i2 = 2*i;
        data[i] = (char_data[i2+1] << 8) | char_data[i2];
    }
}
