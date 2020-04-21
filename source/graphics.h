#ifndef _CODE_GRAPHICS_H_
#define _CODE_GRAPHICS_H_

#include "game.h"

/* Size of game screen */
#define GAME_WIDTH 160

/* Size of a block */
#define BLOCK_LENGTH 10
#define BLOCK_HEIGHT 8

/* Length of the pad */
#define PAD_LENGTH 35
/* Height of the pad */
#define PAD_HEIGHT 140

/* Set a pixel. */
void set_pixel(unsigned short x, unsigned short y, unsigned short color);

/* Draw a block. */
void draw_block(unsigned short x, unsigned short y, unsigned short color);

/* Draw a line which is a part of number. */
void draw_line(unsigned short x, unsigned short y, bool is_white, bool is_sideway);




/* Draw the initial screen */
void gfx_init(struct game_status* state);

/* Erase a block */
void gfx_delete_block(unsigned short x, unsigned short y, unsigned short color);

// /* Erase the ball from its old position and draw in the new one */
// void gfx_update_ball(const struct ball_status* ball_state);

/* Erase the pad from its old position and draw in the new one */
void gfx_update_pad(const struct pad_status* pad_state);

/* Draw the new score */
void gfx_update_score(unsigned int score);

/* Draw the game over screen */
void gfx_draw_game_over();

/* Draw the game clear screen */
void gfx_draw_game_clear();

#endif