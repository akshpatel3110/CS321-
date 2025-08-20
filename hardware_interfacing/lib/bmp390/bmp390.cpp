#include "bmp390.h"
#include <Arduino.h>
#include <Adafruit_BMP3XX.h>

#define SEALEVELPRESSURE_HPA (1015.20) // Barometric Pressure in my location

// Create an instance of the BMP3XX pressure and altitude sensor
static Adafruit_BMP3XX bmp;

// Function to initialize the BMP390 sensor
bool bmp390_init()
{
    // Serial.println("BMP390 test");

    // Initialize the BMP390 sensor using I2C communication
    // The BMP390 sensor is initialized with the default address (0x76).
    // The bmp.begin_I2C() function initializes the sensor and returns false if it fails
    if (!bmp.begin_I2C())
    {
        // If the sensor is not found, print an error message and halt the program
        Serial.println("Couldn't find BMP390 sensor");
        while (1)
        {
            delay(10);
        }
    }
    Serial.println("Found BMP390 sensor");

    // Set up oversampling and filter initialization
    bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X); // 4x pressure oversampling
    bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);    // IIR filter coefficient (Medium Noise)
    bmp.setOutputDataRate(BMP3_ODR_50_HZ);             // Output data rate set to 50 Hz
    return true;
}

// Function to read pressure and altitude from the BMP390 sensor
void bmp390_read(float &pressure_hpa, float &altitude_m)
{
    // Perform a reading from the BMP390 sensor
    // If reading fails, print an error message and return
    // The performReading() function fills the bmp.pressure and bmp.temperature variables
    if (!bmp.performReading())
    {
        Serial.println("Failed to perform reading :(");
        return;
    }
    else
    {
        // The pressure is converted from Pa to hPa by dividing by 100.0
        pressure_hpa = bmp.pressure / 100.0;

        // The altitude is calculated based on the sea level pressure using the readAltitude function from the BMP390 library.
        altitude_m = bmp.readAltitude(SEALEVELPRESSURE_HPA);
    }
}
