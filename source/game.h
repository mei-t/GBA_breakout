#ifndef _CODE_GAME_H_
#define _CODE_GAME_H_

#include "graphics.h"

/* If the button pressed or not. */
bool is_pressed(unsigned short BUTTON, unsigned short buttons);

void hit_block(unsigned short x, unsigned short y, unsigned short color, int *score);

bool can_go_horizontal(unsigned short x, unsigned short y, bool is_left, int* score);

bool can_go_vertical(unsigned short x, unsigned short y, bool is_up, int *score);

void check_next(struct ball_status *bs, int *score);

void define_ball_orbit(struct ball_status *bs, int *score);

#endif