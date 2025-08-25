#ifndef LORA_SX1276_H
#define LORA_SX1276_H
#include <Arduino.h>

/* ==== Radio config ==== */
#ifndef RADIO_FREQ_HZ          // set for your region: 915E6 (US), 868E6 (EU)...
#define RADIO_FREQ_HZ   915E6
#endif
#ifndef RADIO_TX_DBM
#define RADIO_TX_DBM    17     // 2..20 depending on board/regulatory limits
#endif
#ifndef RADIO_BW_HZ
#define RADIO_BW_HZ     125E3  // 125 kHz
#endif
#ifndef RADIO_SF
#define RADIO_SF        7      // 6..12
#endif
#ifndef RADIO_CR
#define RADIO_CR        5      // coding rate denominator: setCodingRate4(5) => 4/5
#endif
#ifndef RADIO_PREAMBLE
#define RADIO_PREAMBLE  8
#endif
#ifndef RADIO_SYNCWORD
#define RADIO_SYNCWORD  0x34   // LoRaWAN-like public sync word (0x34)
#endif

extern String oledline[9];
bool lora_sx1276_init();
void lora_sx1276_transmit(const char*);

#endif