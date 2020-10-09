#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>

/* notes and their TIMER1_TOP */
#define G4 321
#define C5 240
#define E5 190
#define G5 160

typedef struct {
    uint16_t num;
    int samples[];
} Note;

/**
 * 
 **/
typedef struct {
    uint16_t len;
    uint16_t notes[];
} Song;

typedef struct {
    uint8_t num_songs;
    Song* songs[];
} Jukebox;

void advance_music();
void start_song(int song);
void stop_song();

extern uint8_t sinewave[];

#endif
