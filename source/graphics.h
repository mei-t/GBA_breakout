#ifndef _CODE_GRAPHICS_H_
#define _CODE_GRAPHICS_H_

#include <stdbool.h>

/* Size in pixels of screen in mode 3 */
#define MODE3_WIDTH 240
#define MODE3_HEIGHT 160

/* Video memory */
#define VRAM ((volatile unsigned short *)0x06000000)

/* Size of game screen */
#define GAME_WIDTH 160

/* Size of a block */
#define BLOCK_LENGTH 10
#define BLOCK_HEIGHT 8

/* Length of the pad */
#define PAD_LENGTH 35
/* Height of the pad */
#define PAD_HEIGHT 140

/* The number of blocks */
#define SIDEWAYS_BLOCKS 8
#define LENGTHWAYS_BLOCKS 3

// struct ball_status{
//     unsigned short x;
//     unsigned short y;
//     bool is_up;
//     bool is_left;
// };

// struct pad_status {
//     unsigned short x;
//     unsigned short y;
// };

struct game_state {
    bool block[SIDEWAYS_BLOCKS][LENGTHWAYS_BLOCKS];

    struct ball_status{
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

/* Set a pixel. */
void set_pixel(unsigned short x, unsigned short y, unsigned short color);

/* Draw a block. */
void draw_block(unsigned short x, unsigned short y, unsigned short color);

/* Draw a line which is a part of number. */
void draw_line(unsigned short x, unsigned short y, bool is_white, bool is_sideway);

/* Draw score on screen. */
void draw_score(int score);


/* Delete the block which the ball hit. */
void delete_block(unsigned short x, unsigned short y, unsigned short color);



/* Draw the initial screen */
void gfx_init(struct game_state* state);
// /* Erase a block */
// void gfx_delete_block(unsigned short x, unsigned short y);
// /* Erase the ball from its old position and draw in the new one */
// void gfx_update_ball(const struct ball_status* ball_state);
// /* Erase the pad from its old position and draw in the new one */
// void gfx_update_pad(const struct pad_status* pad_state);
// /* Draw the new score */
// void gfx_update_score(unsigned int score);

/* Draw the game over screen */
void gfx_draw_game_over();

/* Draw the game clear screen */
void gfx_draw_game_clear();

#endif