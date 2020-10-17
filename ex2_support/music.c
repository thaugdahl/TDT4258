#include "music.h"
#include "timer.h"
#include "utils.h"
#include "sinewave.h"
#include "notes.h"
uint16_t sinewave[SINEWAVE_LENGTH] = SINEWAVE;


/**
 * Songs
 * Defined in the following format: {length, tempo, notes[]}
 * 
 * 0 is empty note. The notes are defined in notes.h  \
 * with values that should be written to the TIMER1_TOPB register \
 * to result in the correct frequency.
 */

song_t mario_theme_start = { 25, 14, {E5, 0, E5, 0, 0, 0, E5, 0, 0, 0, C5, 0, E5,0, 0, 0, G5, 0, 0, 0, 0, 0, 0, 0, G4} };

song_t mario_long_theme =  {
201, 14, { 
G3, 0, G3, 0, 0, 0, G3, 0, 0, 0, DS3, 0, G3, 0, 0, 0, AS3, 0, 0, 0, 0,
0, 0, 0, AS2, 0, 0, 0, 0, 0, 0, 0, DS3, 0, 0, 0, 0, 0, 0, 0, AS2, 0,
0, 0, G2, 0, 0, 0, 0, 0, C3, 0, 0, 0, D3, 0, 0, 0, CS3, 0, C3, 0, 0,
0, AS2, 0, 0, G3, 0, AS3, 0, 0, C4, 0, 0, 0, GS3, 0, AS3, 0, 0, 0, G3, 0,
0, 0, DS3, 0, F3, 0, D3, 0, 0, 0, 0, 0, DS3, 0, 0, 0, 0, 0, AS2, 0, 0,
0, 0, 0, G2, 0, 0, 0, 0, 0, C3, 0, 0, 0, D3, 0, 0, 0, CS3, 0, C3, 0,
0, 0, AS2, 0, 0, G3, 0, AS3, 0, 0, C4, 0, 0, 0, GS3, 0, AS3, 0, 0, 0, G3,
0, 0, 0, DS3, 0, F3, 0, D3, 0, 0, 0, 0, 0, 0, 0, 0, 0, AS3, 0, A3, 0,
GS3, 0, FS3, 0, 0, 0, G3, 0, 0, 0, B2, 0, C3, 0, DS3, 0, 0, 0, C3, 0, DS3,
0, F3, 0, 0, 0, 0, 0, AS3, 0, A3, 0, 0
} };
 
song_t star_wars =  {98, 14, { G3, 0, G3, 0, G3, 0, C4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, G4, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, F4, 0, E4, 0, D4, 0, C5, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, G4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, F4, 0, E4,
 0, D4, 0, C5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, G4, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, F4, 0, E4, 0, F4, 0, D4} };

song_t undertale_theme =  {198, 10, { D4, D4, D5, 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4, G4, C4, C4, D5, 0, A4,
 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4, G4, B3, B3, D5, 0, A4, 0, 0, GS4, 0, G4,
 0, F4, 0, D4, F4, G4, AS3, AS3, D5, 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4,
 G4, D4, D4, D5, 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4, G4, C4, C4, D5, 0,
 A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4, G4, B3, B3, D5, 0, A4, 0, 0, GS4, 0,
 G4, 0, F4, 0, D4, F4, G4, AS3, AS3, D5, 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4,
 F4, G4, D4, D4, D5, 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4, G4, C4, C4, D5,
 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0, D4, F4, G4, B3, B3, D5, 0, A4, 0, 0, GS4,
 0, G4, 0, F4, 0, D4, F4, G4, AS3, AS3, D5, 0, A4, 0, 0, GS4, 0, G4, 0, F4, 0,
 D4, F4, G4, D4, D4, D5, 0, A4, 0} };

song_t godfather_theme =  {184, 5, { A4, 0, D5, 0, F5, 0, E5, 0, D5, 0, F5, 0, D5, 0, E5, 0, D5, 0, AS4, 0, C5,
 0, A4, A4, A4, A4, 0, 0, 0, 0, 0, 0, A4, 0, D5, 0, F5, 0, E5, 0, D5, 0,
 F5, 0, D5, 0, E5, 0, D5, 0, AS4, 0, A4, 0, G4, G4, G4, G4, 0, 0, 0, 0, 0,
 0, G4, 0, AS4, 0, CS5, 0, E5, E5, E5, E5, 0, 0, 0, 0, 0, 0, G4, 0, AS4, 0,
 CS5, 0, D5, D5, D5, D5, 0, 0, 0, 0, 0, 0, D4, 0, E4, 0, C5, 0, AS4, 0, A4,
 0, C5, 0, AS4, 0, AS4, 0, A4, 0, A4, 0, CS4, 0, D4, D4, D4, D4, 0, 0, 0, 0,
 0, 0, D5, 0, D5, 0, CS5, 0, C5, C5, C5, C5, 0, 0, 0, 0, 0, 0, E5, 0, D5,
 0, AS4, 0, A4, A4, A4, A4, 0, 0, 0, 0, 0, 0, A4, 0, C5, 0, A4, 0, G4, G4,
 G4, G4, 0, 0, 0, 0, 0, 0, G4, 0, AS4, 0, GS4, 0, A4} };


static song_t* active_song; 
static uint8_t i = 0;

static jukebox_t jukebox = {
    .num_songs = 5,
    .songs = {&mario_theme_start, &mario_long_theme, &undertale_theme, &star_wars, &godfather_theme}
};

/**
* function to start a song
* @param song an integer that corresponds to the correct song in jukebox
* Sets the active song and starts the NoteTimer with the correct tempo. 
*/
void start_song(int song)
{

    if(active_song !=0)
    {
        stop_song();
    }

    int song_index = clamp(song,0,jukebox.num_songs-1);

    active_song = jukebox.songs[song_index];
    i = 0;
    setNoteFreq(jukebox.songs[song_index]->tempo);
    startNoteTimer();


    
}

/**
* function to stop a song
* Sets active song stops all timers 
*/
void stop_song()
{
    active_song = 0;
    stopNoteTimer();
    stopSamplingTimer();
    i = 0;
}
/**
* Play next note in song
* If the next note is 0, play no sound
*/
void advance_music()
{
    switch (active_song->notes[i])
    {
        case 0:
            stopSamplingTimer();
            break;
        default:
            samplingTimer_setTop(active_song->notes[i]);
            startSamplingTimer();
            break;
    }
    i++;
    if (i > active_song->len)
    {
        stop_song();
    }
}
