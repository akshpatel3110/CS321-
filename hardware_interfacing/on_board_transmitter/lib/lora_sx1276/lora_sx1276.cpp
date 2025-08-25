#include "lora_sx1276.h"
#include <Arduino.h>
#include "ssd1306.h"
#include <SPI.h>
#include <LoRa.h>

/* ==== Board pinout (T3 V1.6.x, SX1276) ==== */
static constexpr int LORA_SCK  = 5;
static constexpr int LORA_MISO = 19;
static constexpr int LORA_MOSI = 27;
static constexpr int LORA_CS   = 18;
static constexpr int LORA_RST  = 23;
static constexpr int LORA_DIO0 = 26;
// Optional refs:
static constexpr int LORA_DIO1 = 33;
static constexpr int LORA_DIO2 = 32;

static int counter = 0;

bool lora_sx1276_init() {
  // Wire up the SPI peripheral to VSPI pins used by the radio
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);

  // Chip-select, reset, and DIO0 for the LoRa transceiver
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(RADIO_FREQ_HZ)) {
    Serial.println("[LoRa] begin() failed. Check wiring and frequency.");
    while (true) { delay(100); }
  }

  // Optional tuning: SF/BW/CR/Preamble/Syncword/Tx power
  LoRa.setSpreadingFactor(RADIO_SF);           // 6..12
  LoRa.setSignalBandwidth(RADIO_BW_HZ);        // e.g., 125E3
  LoRa.setCodingRate4(RADIO_CR);               // denominator (5 -> 4/5)
  LoRa.setPreambleLength(RADIO_PREAMBLE);
  LoRa.setSyncWord(RADIO_SYNCWORD);
  LoRa.setTxPower(RADIO_TX_DBM);               // may use PA_BOOST internally

  // Optional: reduce automatic gain, CRC, etc.
  LoRa.enableCrc();

  Serial.println("[LoRa] init OK");
  Serial.print("[LoRa] Freq: "); Serial.println((double)RADIO_FREQ_HZ, 0);
  return true;
}

void lora_sx1276_transmit(const char *line)
{
  // send packet
  LoRa.idle();
  LoRa.beginPacket();
  LoRa.print(line);
  LoRa.endPacket();
  Serial.println("[TX]" + String(line));
  oledline[1] = String("OnBoard-Sending:") + String(counter);
  counter++;
  displayTextOLED(oledline);
}