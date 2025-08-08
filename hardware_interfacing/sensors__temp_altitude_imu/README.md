# Sensor Interfacing

- A simple program demonstrating the interfacing of multiple I2C-based sensors with an ESP32  based microcontroller using the Arduino core in the PlatformIO environment. 
- The system integrates SHTC3 (temperature and humidity), MPU6050 (acceleration and gyroscope), and BMP390 (pressure and altitude) sensors. Acquired sensor data is displayed in real time on a 128x64 SSD1306 On-Board OLED screen.

## Hardware Components
### Microcontroller
- [LILYGO LoRa32 915Mhz ESP32 Development Board OLED 0.96 Inch SD Card BLE WiFi TTGO Paxcounter Module](https://lilygo.cc/products/lora3) - An ESP32-based development board featuring built-in LoRa 915 MHz radio, a 0.96-inch OLED display, microSD card slot, and onboard Wi-Fi + Bluetooth capabilities. Ideal for low-power long-range IoT applications such as mesh networks, environmental monitoring, and Paxcounter-style projects. [github](https://github.com/Xinyuan-LilyGO/LilyGo-LoRa-Series/blob/master/docs/en/t3_v161_sx1276/t3_v161_sx1276_hw.md)

### Sensors
- [BMP390](https://www.adafruit.com/product/4816) - High-precision, low-noise barometric pressure and altitude sensor.

- [MPU-6050](https://www.adafruit.com/product/3886) - A 6-axis motion sensor combining a 3-axis accelerometer and 3-axis gyroscope for tracking motion and orientation.
- [Sensirion SHTC3](https://www.adafruit.com/product/4636) - Low-power, high-accuracy digital sensor for measuring temperature and humidity.
> **Note:** All sensors are breakout boards from Adafruit with good libraryy support.
---
## Installation
- Install the CH9102 USB bridge driver for the first time. [Windows](https://www.wch-ic.com/downloads/CH343SER_ZIP.html), [Mac](https://www.wch-ic.com/downloads/CH34XSER_MAC_ZIP.html)
- Install [Visual Studio Code](https://code.visualstudio.com/).
- Open VS code and Go to Extensions.
- Search for "**PlatformIO IDE**" and install.
- Wait for Dependencies to Install.
    - PlatformIO will automatically install:
        - PlatformIO Core
        - Python environment
        - Required tools and boards
        - ⚠️ This may take a few minutes (especially the first time).
- Restart VS Code (if prompted)
- Verify Installation - After restarting, you’ll see a PlatformIO icon (alien head) on the sidebar.
---
## How to Run
- Click “**New Project**” in PlatformIO (PIO) Home.
- In the Project Wizard, provide:
    - Project name.
    - Board:     ESP32 Pico Kit (Espressif)
    - Framework: Arduino
    - Location 
    - ⚠️ This may take a few minutes (especially the first time).
- In PIO Home, go to the Libraries section in the PlatformIO sidebar. Search and add the following libraries to the project.
    - Adafruit SHTC3 Library
    - Adafruit MPU6050
    - Adafruit Unified Sensor
    - Adafruit BMP3XX Library
    - Adafruit SSD1306 (optional, if you want OLED display).
- The project structure will be something like:
    - `.pio/`
    - `.vscode/`
    - `include/`
    - `lib/`
    - `src/main.cpp`
    - `platformio.ini`
- Copy and Paste the source code into the project's `main.cpp` file.
- Click the (**✔**) symbol in the lower left corner to compile/Build.
- Connect the board to the computer USB , Micro-USB is used for module fimware upgrade.
- Click (**→**) to upload firmware.
- Click on (**Upload and Monitor**) to upload firmware and monitor the Serial Monitor.
---
## Sensor Interfacing and Data Display
### 1. Sensor Interfacing (I²C Communication)
    - All sensors (SHTC3, MPU6050, BMP390) use the I2C protocol.
    - Wire.begin(21, 22) sets GPIO21 as SDA and GPIO22 as SCL on the ESP32.
    - Each sensor is connected in parallel on the same I2C bus and powered via 3.3V and GND.
    - The I2C addresses are handled internally by the Adafruit libraries.

### 2. Sensor Initialization
- **SHTC3**: 
    - Initialized using **shtc3.begin()** to prepare temperature and humidity readings.
- **MPU6050**: 
    - Initialized using **mpu.begin()**.
    - Accelerometer range: ±8g, Gyroscope range: ±500°/s, Digital low-pass filter: 21Hz.
- **BMP390**:
    - Initialized using **bmp.begin_I2C()**.
    - Pressure oversampling is set to 4x, IIR filter to medium (coeff 3), and output data rate to 50 Hz.

### 3. Sensor Data Acquisition
- Temperature & Humidity (**SHTC3**):
    - **shtc3.getEvent(&humidity, &temp)** reads both values.
    - Values are printed to the serial monitor and stored in oledline[2] for OLED display.
- Acceleration & Gyroscope (**MPU6050**):
    - **mpu.getEvent(&accel, &gyro, &throwaway)** fetches latest accelerometer and gyroscope readings.
    - Acceleration stored in oledline[4], gyro data in oledline[5].
- Pressure & Altitude (**BMP390**):
    - **bmp.performReading()** ensures fresh data.
    - Pressure (in hPa) stored in oledline[7], altitude (based on sea-level pressure constant) in oledline[8].

### 4. OLED Display Output
- The SSD1306 OLED (128x64) is initialized with address 0x3C.
- The screen is updated every loop cycle (~1s).
- The function displayTextOLED():
    - Clears the screen.
    - Sets text size and color.
    - Iterates through oledline[1] to oledline[8], printing sensor data line-by-line.
    - myOled.display() pushes the updates to the OLED.

### 5. Visual and Serial Output
- Sensor data is shown both on:
    - Serial monitor via Serial.print() (for debugging/logging).
    - OLED screen for real-time embedded visualization.
---
## Wiring & Pictures
![Wiring](sensors__temp_altitude_imu/images/wiring.png)

Few pictures
![1](hardware_interfacing/sensors__temp_altitude_imu/images/pic1.jpeg)
![2](hardware_interfacing/sensors__temp_altitude_imu/images/pic2.jpeg)
![3](hardware_interfacing/sensors__temp_altitude_imu/images/pic3.jpeg)
---


