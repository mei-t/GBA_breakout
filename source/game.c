#include "game.h"
#include "graphics.h"

void game_init(struct game_status* state){
    // Initialize state.
    for(int i = 0; i < SIDEWAYS_BLOCKS; i++){
        for(int j = 0; j < LENGTHWAYS_BLOCKS; j++){
            state->block[i][j] = true;
        }
    }
    struct ball_status ball = {GAME_WIDTH/2, PAD_HEIGHT-1, true, false};
    state->ball = ball;
    struct pad_status pad = {GAME_WIDTH/2, PAD_HEIGHT};
    state->pad = pad;
    state->score = 0;

}

bool is_pressed(unsigned short BUTTON, unsigned short buttons){
    return (BUTTON & buttons) == 0;
}

typedef enum { NOTHING, BLOCK, WALL } collision;

static bool is_wall_hit(unsigned short x, unsigned short y) {
    return (x == 0 || x >= GAME_WIDTH || y == 0 || y >= MODE3_HEIGHT - 1);
}

static bool is_pad_hit(const struct game_status* state, unsigned short x, unsigned short y) {
    if(y == state->pad.y - 1 && x > state->pad.x - PAD_LENGTH/2 && x < state->pad.x + PAD_LENGTH/2)
        return true;
    return false;
}

static collision can_go_horizontal(const struct ball_status* ball_state){
    unsigned short x = ball_state->x;
    ball_state->is_left ? x-- : x++;
    unsigned short next_color = VRAM[ball_state->y*MODE3_WIDTH + x];
    switch (next_color) {
        case 0x0: return NOTHING;
        case 0xFFFF: return WALL;
        default: return BLOCK;
    }
}

static collision can_go_vertical(const struct ball_status* ball_state){
    unsigned short y = ball_state->y;
    ball_state->is_up ? y-- : y++;
    unsigned short next_color = VRAM[y*MODE3_WIDTH + ball_state->x];
    switch (next_color) {
        case 0x0: return NOTHING;
        case 0xFFFF: return WALL;
        default: return BLOCK;
    }
}

void define_ball_orbit(struct game_status* state){
    bool block_hit = false;
    collision col = can_go_horizontal(&state->ball);
    if(col != NOTHING) {
        if(col == BLOCK) {
            unsigned short next_x = state->ball.x + (state->ball.is_left ? -1 : 1);
            block_hit = true;
            gfx_delete_block(next_x, state->ball.y, 0x7C00);
        }
        state->ball.is_left = !state->ball.is_left;
    }

    col = can_go_vertical(&state->ball);
    if(col != NOTHING) {
        if(col == BLOCK) {
            unsigned short next_y = state->ball.y + (state->ball.is_up ? -1 : 1);
            block_hit = true;
            gfx_delete_block(state->ball.x, next_y, 0x7C00);
        }
        state->ball.is_up = !state->ball.is_up;
    }

    if(block_hit) {
        state->score++;
        gfx_update_score(state->score);
    }
}

void update_ball(struct ball_status* ball_state){
    ball_state->is_up ? ball_state->y-- : ball_state->y++;
    ball_state->is_left ? ball_state->x-- : ball_state->x++;
}