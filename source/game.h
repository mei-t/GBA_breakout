#ifndef _CODE_GAME_H_
#define _CODE_GAME_H_

#include <stdbool.h>

/* The number of blocks */
#define SIDEWAYS_BLOCKS 10
#define LENGTHWAYS_BLOCKS 3

struct game_status {
    bool block[SIDEWAYS_BLOCKS * LENGTHWAYS_BLOCKS];

    struct ball_status {
        unsigned short x;
        unsigned short y;
        bool is_up;
        bool is_left;
    } ball;

    struct pad_status {
        unsigned short x;
        unsigned short y;
    } pad;

    unsigned int score;
};

enum game_state {
    PLAYING,
    GAME_WON,
    GAME_OVER,
};

/* Initialize game status. */
void game_init(struct game_status* state);

/* Handle input. */
void game_handle_input(struct game_status* state);

enum game_state game_update(struct game_status* state);

#endif