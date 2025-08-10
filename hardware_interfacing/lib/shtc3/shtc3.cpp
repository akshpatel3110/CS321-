#include "shtc3.h"
#include <Arduino.h>
#include <Adafruit_SHTC3.h>

// Create an instance of the SHTC3 temperature & humidity sensor
static Adafruit_SHTC3 shtc3; // private to this file

// Function to initialize the SHTC3 Temperature and Humidity sensor
bool shtc3_init() {
    
    // Serial.println("SHTC3 test");

    // Initialize the SHTC3 sensor using I2C communication
    // The SHTC3 sensor is initialized with the default address (0x70).
    // The shtc3.begin() function initializes the sensor and returns false if it fails
    if (!shtc3.begin()) {

        // If the sensor is not found, print an error message and halt the program
        Serial.println("Couldn't find SHTC3");
        while (1) {
            delay(10);
        }
    }
    Serial.println("Found SHTC3 sensor");
    return true;
}

// Function to read temperature and humidity from the SHTC3 sensor
void shtc3_read(float &temperature, float &humidity) {

    // Create an event structure to hold the temperature and humidity data
    sensors_event_t humidity_event, temp_event;

    // The getEvent method fills the event structure with the temperature and humidity data
    shtc3.getEvent(&humidity_event, &temp_event);

    temperature = temp_event.temperature;
    humidity = humidity_event.relative_humidity;
}
