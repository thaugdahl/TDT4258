#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
void setupSamplingTimer(uint16_t period, uint8_t prescaler);
void setSamplingFrequency(uint32_t frequency);
void enableSamplingTimerInterrupt();
void startSamplingTimer();
void stopSamplingTimer();

#endif
