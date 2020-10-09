#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
void setupSamplingTimer(uint16_t period, uint8_t prescaler);
void setSamplingFrequency(uint32_t frequency);
void startSamplingTimer();
void stopSamplingTimer();
void setupSemiquiverTimer(uint16_t period, uint8_t prescaler);
void startSemiquiverTimer();
void stopSemiquiverTimer();

void enableTimerInterrupts();
#endif
