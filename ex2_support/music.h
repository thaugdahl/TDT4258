#include <stdint.h>

/* TIMER1_TOP */
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

typedef struct Jukebox {
    uint8_t num_songs;
    Song* songs[];
} Jukebox;


void init_jukebox();

void advance_music();
void start_song(int song);

// ============================================================
// Songs
// 0 is empty semiquaver
// ============================================================
Song mario_theme_start = { 16, {E5, 0, E5, 0, E5, 0, C5, 0, E5, 0, G5, 0, 0, 0, G4} };