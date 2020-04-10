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

int number[10][7] = {
    {1, 0, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 1, 0},
    {1, 1, 1, 0, 1, 0, 1},
    {0, 1, 0, 1, 1, 0, 1},
    {1, 1, 1, 1, 0, 0, 1},
    {1, 1, 1, 1, 0, 1, 1},
    {1, 0, 0, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 0, 1}
};

int letter_g[12][9] = {
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0}
};

/* Set a pixel. */
void set_pixel(unsigned short x, unsigned short y, unsigned short color);

/* Draw a block. */
void draw_block(unsigned short x, unsigned short y, unsigned short color);

/* Draw a line which is a part of number. */
void draw_line(unsigned short x, unsigned short y, bool is_white, bool is_sideway);

/* Draw score on screen. */
void draw_score(int score);

/* Draw default screen. */
void init(int *score, int pad_x);

/* Delete the block which the ball hit. */
void delete_block(unsigned short x, unsigned short y, unsigned short color);

/* Draw "GAME OVER" on screen. */
void draw_game_over();

/* Draw "GAME CLEAR" on screen. */
void draw_game_clear();

#endif