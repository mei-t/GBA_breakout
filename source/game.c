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

// typedef enum { NOTHING, BLOCK, WALL } collision;

static bool is_wall_hit(unsigned short x, unsigned short y) {
    return (x == 0 || x >= GAME_WIDTH || y == 0 || y >= MODE3_HEIGHT - 1);
}

static bool is_pad_hit(const struct game_status* state, unsigned short x, unsigned short y) {
    return (y == state->pad.y && x > state->pad.x - PAD_LENGTH/2 && x < state->pad.x + PAD_LENGTH/2);
}

static unsigned int get_block_x_index(unsigned int x) {
    // VRAM[(x - BLOCK_MARGIN_X) / (BLOCK_GAP + BLOCK_LENGTH)] = 0x03E0;
    // if((x - BLOCK_MARGIN_X) / (BLOCK_GAP + BLOCK_LENGTH) == 9)
    //     VRAM[(x - BLOCK_MARGIN_X) / (BLOCK_GAP + BLOCK_LENGTH)] = 0x03E0;
    return (x - BLOCK_MARGIN_X) / (BLOCK_GAP + BLOCK_LENGTH);
}

static unsigned int get_block_y_index(unsigned int y) {
    // if((y - 1 - BLOCK_MARGIN_Y) / (BLOCK_GAP + BLOCK_HEIGHT) == 2)
    //     VRAM[(y - BLOCK_MARGIN_Y) / (BLOCK_GAP + BLOCK_HEIGHT)] = 0x001F;
    return (y - BLOCK_MARGIN_Y) / (BLOCK_GAP + BLOCK_HEIGHT);
}

static bool is_block_hit(bool* block, unsigned short x, unsigned short y) {
    unsigned short block_max_x = BLOCK_MARGIN_X + (BLOCK_GAP + BLOCK_LENGTH) * SIDEWAYS_BLOCKS - BLOCK_GAP;
    unsigned short block_max_y = BLOCK_MARGIN_Y + (BLOCK_GAP + BLOCK_HEIGHT) * LENGTHWAYS_BLOCKS - BLOCK_GAP;
    if(x <= BLOCK_MARGIN_X || x > block_max_x || y <= BLOCK_MARGIN_Y || y > block_max_y) 
        return false;
    if((x - BLOCK_MARGIN_X) % (BLOCK_GAP + BLOCK_LENGTH) <= BLOCK_LENGTH && (y - BLOCK_MARGIN_Y) % (BLOCK_GAP + BLOCK_HEIGHT) <= BLOCK_HEIGHT)
        return block[get_block_y_index(y) * BLOCK_LENGTH + get_block_x_index(x)];
    // VRAM[15 * MODE3_WIDTH + 15] = 0x03E0;
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
        state->block[get_block_x_index(x)][get_block_y_index(y)] = false;
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

static collision can_go_horizontal(const struct game_status* state, const struct ball_status* ball_state){
    unsigned short x = ball_state->x;
    ball_state->is_left ? x-- : x++;
    return is_colliding(state, x, ball_state->y);
}

static collision can_go_vertical(const struct game_status* state, const struct ball_status* ball_state){
    unsigned short y = ball_state->y;
    ball_state->is_up ? y-- : y++;
    return is_colliding(state, ball_state->x, y);
}

void define_ball_orbit(struct game_status* state){
    bool block_hit = false;
    collision col = can_go_horizontal(state, &state->ball);
    if(col.type != NOTHING) {
        if(col.type == BLOCK) {
            unsigned short next_x = state->ball.x + (state->ball.is_left ? -1 : 1);
            block_hit = true;

            // VRAM[15 * MODE3_WIDTH + 15] = 0x03E0;
            // TODO: replace this function with one that uses col.block_x and col.block_y
            // to delete the block, without using a color.
            gfx_delete_block(col.block_x, col.block_y);
        }
        state->ball.is_left = !state->ball.is_left;
    }

    col = can_go_vertical(state, &state->ball);
    if(col.type != NOTHING) {
        if(col.type == BLOCK) {
            unsigned short next_y = state->ball.y + (state->ball.is_up ? -1 : 1);
            block_hit = true;
            // VRAM[15 * MODE3_WIDTH + 15] = 0x001F;
            gfx_delete_block(col.block_x, col.block_y);
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