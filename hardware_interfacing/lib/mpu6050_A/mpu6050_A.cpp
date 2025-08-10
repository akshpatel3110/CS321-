#include "mpu6050_A.h"
#include <Arduino.h>
#include <Adafruit_MPU6050.h>

// Create an instance of the MPU6050 accelerometer and gyroscope sensor
static Adafruit_MPU6050 mpu;

// Function to initialize the MPU6050 accelerometer + gyroscope sensor
bool mpu6050_init()
{
    // Serial.println("MPU6050 test");

    // Initialize the MPU6050 sensor using I2C communication
    // The MPU6050 sensor is initialized with the default address (0x68).
    // The mpu.begin() function initializes the sensor and returns false if it fails
    if (!mpu.begin())
    {
        // If the sensor is not found, print an error message and halt the program
        Serial.println("Couldn't find MPU6050 sensor");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("Found MPU6050 sensor");

    // Set up accelerometer and gyroscope ranges and filter bandwidth
    mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // Set accelerometer ±8g range
    mpu.setGyroRange(MPU6050_RANGE_250_DEG);      // Set gyroscope ±500 deg/s range
    mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   // Set filter bandwidth to 94 Hz (digital low-pass filter)
    return true;
}

// Function to read accelerometer and gyroscope data from the MPU6050 sensor
void mpu6050_read(float &ax, float &ay, float &az, float &gx, float &gy, float &gz)
{
    // Create an event structure to hold the accelerometer and gyroscope data
    sensors_event_t a, g, throwaway;

    // The getEvent method fills the event structure with the accelerometer and gyroscope data
    // The throwaway variable is used to ignore the temperature data, which is not needed in this case
    mpu.getEvent(&a, &g, &throwaway);
    ax = a.acceleration.x - 0.35; // Adjust for sensor bias
    ay = a.acceleration.y - (-0.1); // Adjust for sensor bias
    az = a.acceleration.z - (-2.31);
    gx = g.gyro.x - (-0.1); // Adjust for sensor bias
    gy = g.gyro.y - (0.0);
    gz = g.gyro.z;
}
