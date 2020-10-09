#include "music.h"
#include "timer.h"
#include "utils.h"

static Song* active_song; // length of longest song/sound
static uint8_t i = 0;

static Jukebox jukebox = {
    .num_songs = 1,
    .songs = {&mario_theme_start}
};

void start_song(int song){
    int song_index = clamp(song,0,jukebox.num_songs-1);

    active_song = jukebox.songs[song_index];
    i = 0;
}

void advance_music(){
    if (i == active_song->len){
        stopSamplingTimer();
        i = 0;
        return;
    }

    switch (active_song->notes[i]){
        case 0:
            stopSamplingTimer();
            break;
        default:
            samplingTimer_setTop(active_song->notes[i]);
            startSamplingTimer();
            break;
    }
    i++;
}
