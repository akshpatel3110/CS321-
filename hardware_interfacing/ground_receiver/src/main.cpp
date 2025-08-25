/***********************************************************************
  This is an example for interfacing the
  SHTC3 Humidity & Temp Sensor,
  MPU-6050 6-DoF Accel and Gyro Sensor and
  BMP390 - Precision Barometric Pressure and Altimeter.


  These sensors use I2C to communicate, 2 pins, SDA and SCL are required
  to interface. 2 other pins Vin and Gnd are also required to provide
  voltage for the sensor to function.
 ***********************************************************************/
#include <Wire.h>
#include <Arduino.h>
#include "ssd1306.h"
#include "lora_sx1276.h"

char tmpBuffer[72];
char tmpBuffer2[72];
char timeBuffer[64];
String oledline[9];
unsigned long currMillis, prevMillis;

void convDDHHMMSS(unsigned long, char *);
void setup()
{
  // Initialize the GPIO pin 25 as an output.
  // This pin can be used to toggle an LED or for other debugging tasks.
  pinMode(25, OUTPUT);
  Serial.begin(115200);
  delay(500);

  // The I2C bus is initialized with the default pins (SDA=21, SCL=22).
  Wire.begin(21, 22);

  // Initialize the OLED display and the sensors
  oled_display_init();
  lora_sx1276_init();

  currMillis = millis();
  // Prepare the OLED display with initial text
  oledline[1] = "Receiving: ";
  for (int jj = 2; jj <= 8; jj++)
  {
    oledline[jj] = "";
  }
  Serial.println("");
  prevMillis = millis();
}

void loop()
{
  currMillis = millis();

  if (currMillis - prevMillis >= 500)
  {
    prevMillis = currMillis;

    // Toggle GPIO pin 25 to indicate that the program is running
    digitalWrite(25, !digitalRead(25));

    lora_sx1276_receive();

    // Display the sensor data on the OLED
    displayTextOLED(oledline);
  }
}