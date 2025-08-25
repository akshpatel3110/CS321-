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
#include "shtc3.h"
#include "mpu6050_A.h"
#include "bmp390.h"
#include "ssd1306.h"
#include "sd_card.h"
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
  shtc3_init();
  mpu6050_init();
  bmp390_init();
  //sd_card_init();
  lora_sx1276_init();

  currMillis = millis();
  // Prepare the OLED display with initial text
  oledline[1] = "OnBoard-Sending: ";
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

    /*** Read SHTC3 temperature and humidity data ***/
    // The temperature is in degrees Celsius and the humidity is in relative humidity percentage.
    float temp, hum;
    shtc3_read(temp, hum);

    // To print on OLED
    sprintf(tmpBuffer, "T:%.1f C | H:%.1f RH%%", temp, hum);
    oledline[2] = tmpBuffer;
    /*** -----------Temperature and Humidity interfacing done -----------***/

    /*** Read MPU6050 accelerometer and gyroscope data ***/
    // The accelerometer data is in m/s^2 and the gyroscope data is in degrees per second.
    float ax, ay, az, gx, gy, gz;
    mpu6050_read(ax, ay, az, gx, gy, gz);

    // To print on OLED
    sprintf(tmpBuffer, "Ac: %.1f, %.1f, %.1f", ax, ay, az);
    oledline[4] = tmpBuffer;
    sprintf(tmpBuffer, "Gyr: %.1f, %.1f, %.1f", gx, gy, gz);
    oledline[5] = tmpBuffer;
    /*** -----------Tri Axial Accelerometer data and Gyroscope data interfacing done--------------- ***/

    /*** Read BMP390 pressure and altitude data ***/
    // Pressure in hPa and Altitude in meters
    float pressure, altitude;
    bmp390_read(pressure, altitude);

    // To print on OLED
    sprintf(tmpBuffer, "Pressure: %.2f hpa", pressure);
    oledline[7] = tmpBuffer;
    sprintf(tmpBuffer, "Altitude: %.2f m", altitude);
    oledline[8] = tmpBuffer;
    /*** -----------Pressure and Altitude data interfacing done----------- ***/

    // Prepare the data to be logged on the SD card
    convDDHHMMSS(currMillis, timeBuffer);
    sprintf(tmpBuffer2, "%s, %.2f,%.2f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.2f,%.2f", timeBuffer, temp, hum, ax, ay, az, gx, gy, gz, pressure, altitude);
    //Serial.println(tmpBuffer2);
    //sd_card_write(tmpBuffer2); // Write the collected sensor data to the SD card

    lora_sx1276_transmit(tmpBuffer2); // Send the collected sensor data via LoRa
    

    // Display the sensor data on the OLED
    displayTextOLED(oledline);
  }
}

void convDDHHMMSS(unsigned long currSeconds, char *uptimeDDHHMMSS) 
{
  int dd, hh, mm, ss, ms;

  ms = currSeconds;
  dd = (ms/86400000UL);
  hh = (ms-(86400000UL*dd))/3600000UL; 
  mm = (ms-(86400000UL*dd)-(3600000UL*hh))/60000UL;
  ss = (ms-(86400000UL*dd)-(3600000UL*hh)-(60000UL*mm))/1000UL;
  ms = (ms-(86400000UL*dd)-(3600000UL*hh)-(60000UL*mm)-(1000UL * ss));

  sprintf(uptimeDDHHMMSS, "%02d:%02d:%02d:%03d", hh, mm, ss, ms);
};
