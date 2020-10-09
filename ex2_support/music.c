#include "music.h"
#include "timer.h"
#include "utils.h"

// Sampled from a signal with freq = 440 Hz
// 440Hz = A4
uint8_t sinewave[] = {
	255, 255, 254, 253, 252, 250, 247, 245,
	242, 238, 234, 230, 226, 221, 216, 210,
	205, 199, 192, 186, 179, 173, 166, 159,
	151, 144, 137, 130, 122, 115, 108, 100,
	93, 86, 79, 73, 66, 60, 54, 48,
	42, 37, 32, 27, 23, 19, 15, 12,
	9, 6, 4, 3, 1, 1, 0, 0,
	0, 1, 3, 4, 6, 9, 12, 15,
	18, 22, 27, 31, 36, 42, 47, 53,
	59, 65, 72, 79, 86, 93, 100, 107,
	114, 121, 129, 136, 143, 151, 158, 165,
	172, 179, 185, 192, 198, 204, 210, 215,
	220, 225, 230, 234, 238, 241, 245, 247,
	250, 251, 253, 254, 255, 255, 255
};

// ============================================================
// Songs
// 0 is empty semiquaver
// ============================================================
Song mario_theme_start = { 16, {E5, 0, E5, 0, E5, 0, C5, 0, E5, 0, G5, 0, 0, 0, G4} };

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
    startSemiquaverTimer();
}

void stop_song(){
    active_song = 0;
    stopSemiquaverTimer();
    stopSamplingTimer();
    i = 0;
}

void advance_music(){
    switch (active_song->notes[i]){
        case 0:
            stopSamplingTimer();
            break;
        default:
            stopSamplingTimer();
            samplingTimer_setTop(active_song->notes[i]);
            startSamplingTimer();
            break;
    }
    i++;
    if (i == active_song->len){
        stop_song();
    }
}
