#ifndef IMU_H__
#define IMU_H__

#include <xc.h>

#define WHO_AM_I        0xF
#define CTRL1_XL        0x10
#define CTRL2_G         0x11
#define CTRL3_C         0x12
#define OUT_TEMP_L      0x20

#define IMU_ADDR        0x6A
#define IMU_WADDR       (IMU_ADDR << 1)
#define IMU_RADDR       (IMU_WADDR | 1)

#define WHO_AM_I_VAL    0b01101001

void imu_setup(void);
void imu_read(signed short* data, int length);

#endif