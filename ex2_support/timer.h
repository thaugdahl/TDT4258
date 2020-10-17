#ifndef TIMER_H
#define TIMER_H

#define NOTE_8HZ_TOP 6835
#define NOTE_1HZ_TOP 54680
    
#include <stdint.h>
void setupSamplingTimer(uint16_t period, uint8_t prescaler);

void setSamplingFrequency(uint32_t frequency);

void startSamplingTimer();

void stopSamplingTimer();

void samplingTimer_setTop(uint16_t top);

void setupNoteTimer();

void startNoteTimer();

void stopNoteTimer();

 
void setNoteFreq(uint16_t freq);

void updateNoteFreq(uint16_t freq);

 
void enableTimerInterrupts();

#endif	/* 
 */
