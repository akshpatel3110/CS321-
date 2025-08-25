#include "ssd1306.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// This file contains the implementation of the SSD1306 OLED display functions
#define SCREEN_WIDTH 128 // OLED display width in pixels
#define SCREEN_HEIGHT 64 // OLED display height in pixels
#define OLED_RESET -1    // Reset pin for the OLED display, -1 means no reset pin is used

// Create an instance of the Adafruit_SSD1306 class
// This instance is used to control the OLED display
static Adafruit_SSD1306 myOled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

bool oled_display_init() {

    // Initialize the OLED display. The begin method initializes the display with the specified VCC state and I2C address
    if (!myOled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

        // If the display is not found, print an error message and halt the program
        Serial.println("SSD1306 allocation failed");
        while (1) {delay(10);}
    }
    Serial.println("Found SSD1306 OLED");
    return true;
}

// Function to display text on the OLED display
void displayTextOLED(String oledline[]) {
    myOled.clearDisplay();              
    myOled.setTextSize(1);                 
    myOled.setTextColor(SSD1306_WHITE); 
    myOled.setCursor(0, 0);             
    for (int jj = 1; jj <= 8; jj++) {   
        myOled.println(oledline[jj]);   
    }
    myOled.display();        
}            
