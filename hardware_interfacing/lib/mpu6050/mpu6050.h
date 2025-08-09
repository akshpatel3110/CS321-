#ifndef MPU6050_H
#define MPU6050_H

bool mpu6050_init();
void mpu6050_read(float &ax, float &ay, float &az, float &gx, float &gy, float &gz);

#endif
