#ifndef MUSIC_H
#define MUSIC_H

#include <stdint.h>

typedef struct {    // TODO: Maybe delete this?
    uint16_t num;
    int samples[];
} Note_t;

/**
 * 
 **/
typedef struct {
    uint16_t len;
    uint16_t notes[];
} song_t;

typedef struct {
    uint8_t num_songs;
    song_t* songs[];
} jukebox_t;

void advance_music();
void start_song(int song);
void stop_song();

extern uint16_t sinewave[];

#endif
