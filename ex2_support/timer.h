#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
void setupTimer(uint16_t period, uint8_t prescaler);
void setSamplingFrequency(uint32_t frequency);
void enableTimerInterrupt();
void startTimer();
void stopTimer();

#endif
