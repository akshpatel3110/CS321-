#ifndef LORA_SX1276_H
#define LORA_SX1276_H
#include <Arduino.h>

/* ==== Radio config (must match sender!) ==== */
#ifndef RADIO_FREQ_HZ
#define RADIO_FREQ_HZ   915E6
#endif
#ifndef RADIO_BW_HZ
#define RADIO_BW_HZ     125E3
#endif
#ifndef RADIO_SF
#define RADIO_SF        7
#endif
#ifndef RADIO_CR
#define RADIO_CR        5
#endif
#ifndef RADIO_PREAMBLE
#define RADIO_PREAMBLE  8
#endif
#ifndef RADIO_SYNCWORD
#define RADIO_SYNCWORD  0x34
#endif

extern String oledline[9];
bool lora_sx1276_init();
void lora_sx1276_receive();

#endif