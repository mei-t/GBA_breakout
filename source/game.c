#include "game.h"
#include "graphics.h"

/* Status of buttons */
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)

void game_init(struct game_status* state){
    // Initialize state.
    for(int i = 0; i < SIDEWAYS_BLOCKS; i++){
        for(int j = 0; j < LENGTHWAYS_BLOCKS; j++){
            state->block[j * BLOCK_LENGTH + i] = true;
        }
    }
    struct ball_status ball = {GAME_WIDTH/2, PAD_HEIGHT-1, true, false};
    state->ball = ball;
    struct pad_status pad = {GAME_WIDTH/2, PAD_HEIGHT};
    state->pad = pad;
    state->score = 0;

}

/* If the button pressed or not. */
static bool is_pressed(unsigned short BUTTON, unsigned short buttons){
    return (BUTTON & buttons) == 0;
}

static bool is_wall_hit(unsigned short x, unsigned short y) {
    return (x == 0 || x >= GAME_WIDTH || y == 0 || y >= MODE3_HEIGHT - 1);
}

static bool is_pad_hit(const struct game_status* state, unsigned short x, unsigned short y) {
    return (y == state->pad.y && x > state->pad.x - PAD_LENGTH/2 && x < state->pad.x + PAD_LENGTH/2);
}

static unsigned int get_block_x_index(unsigned int x) {
    return (x - BLOCK_MARGIN_X) / (BLOCK_GAP + BLOCK_LENGTH);
}

static unsigned int get_block_y_index(unsigned int y) {
    return (y - BLOCK_MARGIN_Y) / (BLOCK_GAP + BLOCK_HEIGHT);
}

static bool is_block_hit(bool* block, unsigned short x, unsigned short y) {
    unsigned short block_max_x = BLOCK_MARGIN_X + (BLOCK_GAP + BLOCK_LENGTH) * SIDEWAYS_BLOCKS - BLOCK_GAP;
    unsigned short block_max_y = BLOCK_MARGIN_Y + (BLOCK_GAP + BLOCK_HEIGHT) * LENGTHWAYS_BLOCKS - BLOCK_GAP;
    if(x <= BLOCK_MARGIN_X || x > block_max_x || y <= BLOCK_MARGIN_Y || y > block_max_y) 
        return false;
    if((x - BLOCK_MARGIN_X) % (BLOCK_GAP + BLOCK_LENGTH) <= BLOCK_LENGTH && (y - BLOCK_MARGIN_Y) % (BLOCK_GAP + BLOCK_HEIGHT) <= BLOCK_HEIGHT)
        return block[get_block_y_index(y) * BLOCK_LENGTH + get_block_x_index(x)];
    return false;
}

typedef struct {
    enum { NOTHING, BLOCK, PAD, WALL } type;
    /* block_x and block_y are set to the coordinates of the block if type is BLOCK */
    unsigned short block_x;
    unsigned short block_y;
} collision;

static collision is_colliding(struct game_status* state, unsigned short x, unsigned short y) {

    if (is_wall_hit(x, y)) {
        collision ret = { WALL };
        return ret;
    }

    if (is_block_hit(state->block, x, y)) {
        collision ret = { BLOCK };
        state->block[get_block_y_index(y) * BLOCK_LENGTH + get_block_x_index(x)] = false;
        ret.block_x = get_block_x_index(x) * (BLOCK_LENGTH + BLOCK_GAP) + BLOCK_MARGIN_X;
        ret.block_y = get_block_y_index(y) * (BLOCK_HEIGHT + BLOCK_GAP) + BLOCK_MARGIN_Y;
        return ret;
    }

    if(is_pad_hit(state, x, y)) {
        collision ret = { PAD };
        return ret;
    }

    collision ret = { NOTHING };
    return ret;
}

static collision can_go_horizontal(struct game_status* state, const struct ball_status* ball_state){
    unsigned short x = ball_state->x;
    ball_state->is_left ? x-- : x++;
    return is_colliding(state, x, ball_state->y);
}

static collision can_go_vertical(struct game_status* state, const struct ball_status* ball_state){
    unsigned short y = ball_state->y;
    ball_state->is_up ? y-- : y++;
    return is_colliding(state, ball_state->x, y);
}

void game_handle_input(struct game_status* state){
    volatile char *ioreg = (char *)0x04000000;
    char buttons = ioreg[0x130];

    // if Right is pressed
    if (is_pressed(BUTTON_RIGHT, buttons) && state->pad.x < GAME_WIDTH - PAD_LENGTH/2 - 1) {
        state->pad.x++;
    }
    // if Left is pressed
    if (is_pressed(BUTTON_LEFT, buttons) && state->pad.x > PAD_LENGTH/2 + 1) {
        state->pad.x--;
    }
}

enum game_state game_update(struct game_status* state){
    bool block_hit = false;
    collision col = can_go_horizontal(state, &state->ball);
    if(col.type != NOTHING) {
        if(col.type == BLOCK) {
            block_hit = true;
            gfx_delete_block(col.block_x, col.block_y);
        }
        state->ball.is_left = !state->ball.is_left;
    }

    col = can_go_vertical(state, &state->ball);
    if(col.type != NOTHING) {
        if(col.type == BLOCK) {
            block_hit = true;
            gfx_delete_block(col.block_x, col.block_y);
        }
        state->ball.is_up = !state->ball.is_up;
    }

    state->ball.is_up ? state->ball.y-- : state->ball.y++;
    state->ball.is_left ? state->ball.x-- : state->ball.x++;
    gfx_update_ball(&state->ball);

    gfx_update_pad(&state->pad);

    if(block_hit) {
        state->score++;
        gfx_update_score(state->score);
    }

    if(state->ball.y == MODE3_HEIGHT-2){
        return GAME_OVER;
    }else if(state->score == SIDEWAYS_BLOCKS * LENGTHWAYS_BLOCKS){
        return GAME_WON;
    }
    return PLAYING;
}