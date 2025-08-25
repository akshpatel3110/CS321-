#include "sd_card.h"
#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

/*** ---- Hardcoded LILYGO T3_V1.6.1 SPI TF/SD pins ---- ***/
#define SD_CS    13
#define SD_MOSI  15
#define SD_SCK   14
#define SD_MISO   2

static bool sd_ready = false;
static const char *kLogPath = "/logdata.txt";

// Function to initialize the SD card communication
bool sd_card_init()
{
    Serial.print("starting SD card... ");
    // Initialize SPI bus for SD card
    // SPI.begin() initializes the SPI bus with the specified pins
    // SD.begin() initializes the SD card and returns false if it fails
    // The SD_CS pin is used as the chip select pin for the SD card
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS))
    {

        // If the card is not found, print an error message and halt the program
        Serial.println("Card Mount Failed");
        return false;
    }
    Serial.println("Found SD card");

    File f = SD.open(kLogPath, FILE_APPEND);
    if (!f)
    {
        Serial.println("failed to open log file for append");
        return false;
    }
    f.close();

    sd_ready = true;
    Serial.printf("logging to %s\n", kLogPath);
    return true;
}

// Function to write a line of text to the SD card
bool sd_card_write(const char *line)
{
    if (!sd_ready || line == nullptr)
        return false;

    File f = SD.open(kLogPath, FILE_APPEND);
    if (!f)
    {
        Serial.println("open() failed in log_data");
        return false;
    }

    // Write the line to the file and get the number of bytes written
    size_t n = f.println(line); // write line + '\n'
    f.flush();
    f.close();

    return (n > 0);
}