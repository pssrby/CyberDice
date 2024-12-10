#ifndef _MPU6050_H_
#define _MPU6050_H_

#define DICE_I2C_SDA_PIN 12
#define DICE_I2C_SCL_PIN 13
void MPU6050_INIT(void);
void mpu6050_read_raw(int16_t accel[3], int16_t gyro[3], int16_t *temp);
#endif