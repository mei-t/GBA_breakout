#include "game.h"
#include "graphics.h"

bool is_pressed(unsigned short BUTTON, unsigned short buttons){
    return (BUTTON & buttons) == 0;
}

void hit_block(unsigned short x, unsigned short y, unsigned short color, int *score){
    gfx_delete_block(x, y, color);
    (*score)++;
    gfx_update_score(*score);
}

bool can_go_horizontal(unsigned short x, unsigned short y, bool is_left, int* score){
    is_left ? x-- : x++;
    unsigned short next_color = VRAM[y*MODE3_WIDTH + x];
    if(next_color == 0x0){
        return false;
    }
    if(next_color != 0xFFFF){
        hit_block(x, y, next_color, score);
    }
    return true;
}

bool can_go_vertical(unsigned short x, unsigned short y, bool is_up, int *score){
    is_up ? y-- : y++;
    unsigned short next_color = VRAM[y*MODE3_WIDTH + x];
    if(next_color == 0x0){
        return false;
    }
    if(next_color != 0xFFFF){
        hit_block(x, y, next_color, score);
    }
    return true;
}

void check_next(struct game_status* state){
    unsigned short next_x = state->ball.x;
    unsigned short next_y = state->ball.y;
    state->ball.is_up ? next_y-- : next_y++;
    state->ball.is_left ? next_x-- : next_x++;
    unsigned short next_color = VRAM[next_y * MODE3_WIDTH + next_x];
    if(next_color == 0x0){
        return;
    }
    if(next_color != 0xFFFF){
        hit_block(next_x, next_y, next_color, &state->score);
    }
    state->ball.is_left = !state->ball.is_left;
    state->ball.is_up = !state->ball.is_up;
}

void define_ball_orbit(struct game_status* state){ // Reference is invalid. Why?
    bool is_straight = true;
    if(can_go_horizontal(state->ball.x, state->ball.y, state->ball.is_left,&state->score)){
        state->ball.is_left = !state->ball.is_left;
        is_straight = false;
    }
    if(can_go_vertical(state->ball.x, state->ball.y, state->ball.is_up, &state->score)){
        state->ball.is_up = !state->ball.is_up;
        is_straight = false;
    }
    if(is_straight){
        check_next(state);
    }
}

void update_ball(struct ball_status* ball_state){
    ball_state->prev_x = ball_state->x;
    ball_state->prev_y = ball_state->y;
    ball_state->is_up ? ball_state->y-- : ball_state->y++;
    ball_state->is_left ? ball_state->x-- : ball_state->x++;
}