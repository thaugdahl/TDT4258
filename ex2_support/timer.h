#ifndef TIMER_H
#define TIMER_H

#define SEMIQUAVER_8HZ_TOP 6835
#define SEMIQUAVER_1HZ_TOP 54680

#include <stdint.h>
void setupSamplingTimer(uint16_t period, uint8_t prescaler);
void setSamplingFrequency(uint32_t frequency);
void startSamplingTimer();
void stopSamplingTimer();
void samplingTimer_setTop(uint16_t top);

void setupSemiquaverTimer();
void startSemiquaverTimer();
void stopSemiquaverTimer();

void setSemiquaverFreq(uint16_t freq);
void updateSemiquaverFreq(uint16_t freq);

void enableTimerInterrupts();
#endif
