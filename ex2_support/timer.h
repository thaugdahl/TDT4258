#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
void setupSamplingTimer(uint16_t period, uint8_t prescaler);
void setSamplingFrequency(uint32_t frequency);
void startSamplingTimer();
void stopSamplingTimer();
void samplingTimer_setTop(uint16_t top);

void setupSemiquaverTimer();
void startSemiquaverTimer();
void stopSemiquaverTimer();

void enableTimerInterrupts();
#endif
