#include "lora_sx1276.h"
#include <Arduino.h>
#include "ssd1306.h"
#include <SPI.h>
#include <LoRa.h>

/* ==== Board pinout (T3 V1.6.x, SX1276) ==== */
static constexpr int LORA_SCK = 5;
static constexpr int LORA_MISO = 19;
static constexpr int LORA_MOSI = 27;
static constexpr int LORA_CS = 18;
static constexpr int LORA_RST = 23;
static constexpr int LORA_DIO0 = 26;
// Optional refs:
static constexpr int LORA_DIO1 = 33;
static constexpr int LORA_DIO2 = 32;

static int counter = 0;
static uint32_t lastRxMs = 0;

bool lora_sx1276_init()
{

  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_DIO0);

  if (!LoRa.begin(RADIO_FREQ_HZ))
  {
    Serial.println("[LoRa] begin() failed. Check wiring and frequency.");
    while (true)
    {
      delay(100);
    }
  }

  LoRa.setSpreadingFactor(RADIO_SF);
  LoRa.setSignalBandwidth(RADIO_BW_HZ);
  LoRa.setCodingRate4(RADIO_CR);
  LoRa.setPreambleLength(RADIO_PREAMBLE);
  LoRa.setSyncWord(RADIO_SYNCWORD);
  LoRa.enableCrc();
  // LoRa.receive();

  Serial.println("[LoRa] init OK");
  Serial.print("[LoRa] Freq: ");
  Serial.println((double)RADIO_FREQ_HZ, 0);
  return true;
}

void lora_sx1276_receive()
{
  int packetSize = LoRa.parsePacket(); // non-blocking
  if (packetSize)
  {
    String incoming;
    incoming.reserve(packetSize);

    while (LoRa.available())
    {
      incoming += (char)LoRa.read();
    }

    long rssi = LoRa.packetRssi();
    float snr = LoRa.packetSnr();

    Serial.print("[RX] '");
    Serial.println(incoming);
    // Serial.print("'  RSSI=");
    // Serial.print(rssi);
    // Serial.print("  SNR=");
    // Serial.println(snr, 1);
    oledline[3] = String(incoming);
    oledline[1] = String("Ground-Receiving:") + String(counter);
    //oledline[4] = String("RSSI=") + String(rssi) + String(" dBm");
    //oledline[5] = String("SNR=") + String(snr, 1) + String(" dB");
    counter++;
    displayTextOLED(oledline);
    lastRxMs = millis();
    LoRa.receive(); // re-arm receiver
  }

  // Optional: watchdog to re-arm RX if the link was idle for a while
  if (millis() - lastRxMs > 5000) {
    LoRa.idle();
    LoRa.receive();
    lastRxMs = millis();
  }
}