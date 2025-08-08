/***********************************************************************
  This is an example for the 
  SHTC3 Humidity & Temp Sensor, 
  MPU-6050 6-DoF Accel and Gyro Sensor and
  BMP390 - Precision Barometric Pressure and Altimeter.


  These sensors use I2C to communicate, 2 pins, SDA and SCL are required 
  to interface. 2 other pins Vin and Gnd are also required to provide 
  voltage for the sensor to function.
 ***********************************************************************/
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SHTC3.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BMP3XX.h"


#define SEALEVELPRESSURE_HPA (1026.4) // Barometric Pressure in my location

#define SCREEN_WIDTH 128 // On-Board OLED display width, in pixels
#define SCREEN_HEIGHT 64 // On-Board OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 myOled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Create an instance of the SHTC3 temperature & humidity sensor
Adafruit_SHTC3 shtc3 = Adafruit_SHTC3();

// Create an instance of the MPU6050 accelerometer and gyroscope sensor
Adafruit_MPU6050 mpu;

// Create an instance of the BMP3XX pressure and altitude sensor
Adafruit_BMP3XX bmp;

char tmpBuffer[72]; //Temporary Buffer
String oledline[9]; //One line of text per OLED row. For textsize of 1 there are 8 OLED rows/lines

void displayTextOLED(String oledline[]);

void setup() {
  pinMode(25, OUTPUT);  // Set GPIO25 as output (used for blinking LED)
  Serial.begin(115200); // Initialize serial communication at 115200 baud rate
  Wire.begin(21, 22);   // Initialize I2C with SDA on GPIO21 and SCL on GPIO22

  // Make sure to use the correct I2C address. Address 0x3C for 128x64
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!myOled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println("SSD1306 allocation failed");
    while(1);{
      delay(10);
    } // If OLED init fails, don't proceed, loop forever
  }
  else{
    Serial.println("Found SSD1306 OLED");
  }

  while (!Serial)
    delay(10);     // Wait for serial connection (for boards like Leonardo, Zero)

  Serial.println("SHTC3 test");
  // Trying to initialize SHTC3 temperature and humidity sensor
  if (! shtc3.begin()) {
    Serial.println("Couldn't find SHTC3");
    while (1) {
      delay(10);   // Halt if sensor not found
    }
  }
  else{
    Serial.println("Found SHTC3 sensor");
  }

  Serial.println("MPU6050 test");
  // Trying to initialize MPU6050 accelerometer + gyroscope sensor
  if (!mpu.begin()) {
    Serial.println("Couldn't find MPU6050 sensor");
    while (1) {
      delay(10);   // Halt if sensor not found
    }
  }
  else{
    Serial.println("Found MPU6050 sensor");
  }

  // Configure MPU6050 sensor settings
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);   // Set accelerometer ±8g range
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);        // Set gyro ±500°/s range
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);     // Set digital low-pass filter

  Serial.println("BMP390 test");
  // Trying to initialize BMP390 Pressure and Altimeter Sensor
  if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    Serial.println("Couldn't find BMP3 sensor");
    while (1) {
      delay(10); // Halt if sensor not found
    }
  }
  else{
    Serial.println("Found BMP3 sensor");
  }

  // Set up oversampling and filter initialization
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_4X);  // 4x pressure oversampling
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_3);     // Medium noise filtering
  bmp.setOutputDataRate(BMP3_ODR_50_HZ);              // Output data rate: 50 Hz

  // Initialize display text buffer
  oledline[1] = "Sensor Data";
  int jj; for (jj=2; jj<=8; jj++){ oledline[jj]=""; }
  Serial.println("");
  delay(100);
}


void loop() {
  digitalWrite(25, !digitalRead(25)); // Toggle LED state 

  /*** Read SHTC3 temperature and humidity ***/
  sensors_event_t humidity, temp;
  shtc3.getEvent(&humidity, &temp); // Populate temp and humidity objects with fresh data
  
  Serial.print("Temperature: "); 
  Serial.print(temp.temperature); 
  Serial.println(" C");

  Serial.print("Humidity: "); 
  Serial.print(humidity.relative_humidity); 
  Serial.println("%RH");

  // Format and store temp + humidity in OLED buffer
  sprintf(tmpBuffer, "T:%.1f C | H:%.1f RH%%", temp.temperature, humidity.relative_humidity);
  oledline[2] = tmpBuffer;

  Serial.println("---------------------------------------------------------------------------");

  /*** Read MPU6050 accelerometer and gyroscope data***/
  sensors_event_t accel, gyro, throwaway;
  mpu.getEvent(&accel, &gyro, &throwaway); // Fetch acceleration and Gyro data

  Serial.println("MPU6050 Accel [m/s^2]"); 
  Serial.print(" X: "); Serial.print(accel.acceleration.x);
  Serial.print(" Y: "); Serial.print(accel.acceleration.y);
  Serial.print(" Z: "); Serial.println(accel.acceleration.z);
  // Format and store Acceleration data in OLED buffer
  sprintf(tmpBuffer, "Acc: %.1f, %.1f, %.1f", accel.acceleration.x, accel.acceleration.y, accel.acceleration.z);
  oledline[4] = tmpBuffer;

  Serial.println("MPU6050 Gyro [rad/s]"); 
  Serial.print(" X: "); Serial.print(gyro.gyro.x);
  Serial.print(" Y: "); Serial.print(gyro.gyro.y);
  Serial.print(" Z: "); Serial.println(gyro.gyro.z);
  // Format and store Gyro data in OLED buffer
  sprintf(tmpBuffer, "Gyro: %.1f, %.1f, %.1f", gyro.gyro.x, gyro.gyro.y, gyro.gyro.z);
  oledline[5] = tmpBuffer;

  Serial.println("---------------------------------------------------------------------------");
  
  /*** Read BMP390 pressure and altitude data ***/ 
  if (! bmp.performReading()) {
    Serial.println("Failed to perform reading :(");
    return; // Skip rest of loop if reading failed
  }

  Serial.print("Pressure = "); 
  Serial.print(bmp.pressure / 100.0); // Converting Pa to hPa
  Serial.println(" hPa");
  // Format and store Pressure data in OLED buffer
  sprintf(tmpBuffer, "Pressure: %.2f hpa", (bmp.pressure / 100.0)); 
  oledline[7] = tmpBuffer;

  Serial.print("Approx. Altitude = "); 
  Serial.print(bmp.readAltitude(SEALEVELPRESSURE_HPA)); // Altitude calculation
  Serial.println(" m");
  // Format and store Altitude data in OLED buffer
  sprintf(tmpBuffer, "Altitude: %.2f m", (bmp.readAltitude(SEALEVELPRESSURE_HPA)));
  oledline[8] = tmpBuffer;
  Serial.println("---------------------------------------------------------------------------");
  
  Serial.println();
  displayTextOLED(oledline); // Display all buffered sensor readings on OLED
  delay(1000);
}

void displayTextOLED(String oledline[]) {
  int jj;
  myOled.clearDisplay();                // Clear previous screen contents
  myOled.setTextSize(1);                // Font size
  myOled.setTextColor(SSD1306_WHITE);   // Font Color (Black or white)
  myOled.setCursor(0, 0);               // Start printing from top-left

  for (jj=1; jj<=8; jj++) { 
    myOled.println(oledline[jj]);       // Print all 8 lines
  }
  
  myOled.display();                     // Push updates to OLED screen
}