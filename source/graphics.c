#include "graphics.h"
#include "game.h"

/* 16-bit volatile register */
#define VR16(x) *((volatile unsigned short *)(x))

#define REG_IOBASE 0x04000000
/* Vertical sync */
#define REG_VCOUNT VR16(REG_IOBASE + 0x6)

static int number[10][7] = {
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

static int letter_g[12][9] = {
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

static int letter_a[12][9] = {
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1}
};

static int letter_m[12][9] = {
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1}
};

static int letter_e[12][9] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1}
};

static int letter_o[12][9] = {
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0}
};

static int letter_v[12][9] = {
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 0, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0, 0}
};

static int letter_r[12][9] = {
    {1, 1, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 0, 0},
    {1, 1, 1, 0, 1, 1, 1, 1, 0},
    {1, 1, 1, 0, 0, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1}
};

static int letter_c[12][9] = {
    {0, 0, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 1, 1, 1, 1, 1, 0},
    {0, 0, 1, 1, 1, 1, 1, 0, 0}
};

static int letter_l[12][9] = {
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// The previous position of the ball. so we can delete it.
static unsigned short prev_ball_x;
static unsigned short prev_ball_y;

static void set_pixel(unsigned short x, unsigned short y, unsigned short color){
    VRAM[y * MODE3_WIDTH + x] = color;
    return;
}

static void draw_block(unsigned short x, unsigned short y, unsigned short color){
    for(int i=0; i<BLOCK_HEIGHT; i++){
        for(int j=0; j<BLOCK_LENGTH; j++){
            set_pixel(x+j, y+i, color);
        }
    }
}

static void draw_line(unsigned short x, unsigned short y, bool is_white, bool is_sideway){
    unsigned short color;
    if(is_white){
        color = 0xFFFF;
    }else{
        color = 0x0;
    }

    for(int i=0; i<5; i++){
        if(is_sideway){
            set_pixel(x + i, y, color);
        }else{
            set_pixel(x, y + i, color);
        }
    }
}

void gfx_init(const struct game_status* state){
    // The video mode setting
    volatile char *ioreg = (char *)0x04000000;
    ioreg[0] = 0x03;
    ioreg[1] = 0x04;

    for(int i=0; i<MODE3_HEIGHT-1; i++){
        set_pixel(0, i, 0xFFFF);
        set_pixel(GAME_WIDTH, i, 0xFFFF);
    }
    for(int i=0; i<GAME_WIDTH+1; i++){
        set_pixel(i, 0, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT-1, 0xFFFF);
    }

    // Draw blocks.
    for(int i=0; i<SIDEWAYS_BLOCKS; i++){
        for(int j=0; j<LENGTHWAYS_BLOCKS; j++){
            if(state->block[j * BLOCK_LENGTH + i]){
                draw_block(i * (BLOCK_LENGTH + BLOCK_GAP) + BLOCK_MARGIN_X, j * (BLOCK_HEIGHT + BLOCK_GAP) + BLOCK_MARGIN_Y, 0x7C00);
            }
        }
    }

    // Draw a pad.
    for(int i=0; i<PAD_LENGTH; i++){
        set_pixel(state->pad.x-PAD_LENGTH/2+i, state->pad.y, 0xFFFF);
    }

    // Draw "SCORE" on screen.
    int score_letter[5][19] = {
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0},
        {1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1},
        {0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0},
        {1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1}
    };
    for(int i=0; i<5; i++){
        for(int j=0; j<19; j++){
            if(score_letter[i][j]){
                set_pixel(j+170, i+130, 0xFFFF);
            }
        }
    }

    gfx_update_score(state->score);

    set_pixel(state->ball.x, state->ball.y, 0xFFFF);
    prev_ball_x = state->ball.x;
    prev_ball_y = state->ball.y;
}

void gfx_delete_block(unsigned short x, unsigned short y){
    for(int i=0; i<BLOCK_HEIGHT; i++){
        for(int j=0; j<BLOCK_LENGTH; j++){
            set_pixel(x+j, y+i, 0x0);
        }
    }
}

void gfx_update_ball(const struct ball_status* ball_state){
    set_pixel(prev_ball_x, prev_ball_y, 0x0);
    set_pixel(ball_state->x, ball_state->y, 0xFFFF);
    prev_ball_x = ball_state->x;
    prev_ball_y = ball_state->y;
}

void gfx_update_pad(const struct pad_status* pad_state){
    set_pixel(pad_state->x - PAD_LENGTH/2, pad_state->y, 0xFFFF);
    set_pixel(pad_state->x + PAD_LENGTH/2, pad_state->y, 0xFFFF);
    unsigned short pad_left = pad_state->x - PAD_LENGTH/2 - 1;
    unsigned short pad_right = pad_state->x + PAD_LENGTH/2 + 1;
    if(pad_left > 0)
        set_pixel(pad_left, pad_state->y, 0x0);
    if(pad_right < GAME_WIDTH)
        set_pixel(pad_right, pad_state->y, 0x0);
}

void gfx_update_score(unsigned int score){
    int digit = 0;
    do{
        int num = score % 10;
        for(int i=0; i<3; i++){
            draw_line(191 - digit * 10, 140 + i * 6, number[num][i], true);
        }
        for(int i=3; i<5; i++){
            draw_line(190 - digit * 10 + (i - 3) * 6, 141, number[num][i], false);
        }
        for(int i=5; i<7; i++){
            draw_line(190 - digit * 10 + (i - 5) * 6, 147, number[num][i], false);
        }
        score /= 10;
        digit++;
    }while(score != 0);
}

static void draw_frame(unsigned short x) {
    for(int j=1; j<GAME_WIDTH; j++){
        set_pixel(j, MODE3_HEIGHT/2 - 7, 0xFFFF);
        set_pixel(j, MODE3_HEIGHT/2 - 6, 0xFFFF);
        set_pixel(j, MODE3_HEIGHT/2 + 7, 0xFFFF);
        set_pixel(j, MODE3_HEIGHT/2 + 8, 0xFFFF);
    }

    for(int i=0; i<12; i++){
        for(int j=1; j<x; j++){
            set_pixel(j, MODE3_HEIGHT/2 - 5 + i, 0xFFFF);
            set_pixel(160 - j, MODE3_HEIGHT/2 - 5 + i, 0xFFFF);
        }
    }
}

static void draw_game(unsigned short x) {
    int start_pos[8] = {x, x + 11, x + 22, x + 33};
    for(int i=0; i<12; i++) {
        for(int j=0; j<9; j++) {
            if(letter_g[i][j] == 0)
                set_pixel(j + start_pos[0], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_a[i][j] == 0)
                set_pixel(j + start_pos[1], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_m[i][j] == 0)
                set_pixel(j + start_pos[2], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_e[i][j] == 0)
                set_pixel(j + start_pos[3], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 3; j++) {
            set_pixel(start_pos[j] + 9, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            set_pixel(start_pos[j] + 10, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
}

void gfx_draw_game_over(){
    draw_frame(36);
    draw_game(36);
    for(int i = 78; i < 84; i++) {
        for(int j = 0; j < 12; j++) {
            set_pixel(i, j + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
    int start_pos[8] = {84, 95, 106, 117};
    for(int i=0; i<12; i++) {
        for(int j=0; j<9; j++) {
            if(letter_o[i][j] == 0)
                set_pixel(j + start_pos[0], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_v[i][j] == 0)
                set_pixel(j + start_pos[1], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_e[i][j] == 0)
                set_pixel(j + start_pos[2], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_r[i][j] == 0)
                set_pixel(j + start_pos[3], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 3; j++) {
            set_pixel(start_pos[j] + 9, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            set_pixel(start_pos[j] + 10, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
}

void gfx_draw_game_clear(){
    draw_frame(30);
    draw_game(30);
    for(int i = 0; i < 12; i++) {
        for(int j = 72; j < 79; j++) {
            set_pixel(j, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
    int start_pos[8] = {79, 90, 101, 112, 123};
    for(int i=0; i<12; i++) {
        for(int j=0; j<9; j++) {
            if(letter_c[i][j] == 0)
                set_pixel(j + start_pos[0], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_l[i][j] == 0)
                set_pixel(j + start_pos[1], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_e[i][j] == 0)
                set_pixel(j + start_pos[2], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_a[i][j] == 0)
                set_pixel(j + start_pos[3], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            if(letter_r[i][j] == 0)
                set_pixel(j + start_pos[4], i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
    for(int i = 0; i < 12; i++) {
        for(int j = 0; j < 4; j++) {
            set_pixel(start_pos[j] + 9, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
            set_pixel(start_pos[j] + 10, i + MODE3_HEIGHT/2 - 5, 0xFFFF);
        }
    }
}

void gfx_wait_end_frame() {
    while (REG_VCOUNT >= MODE3_HEIGHT);
    while (REG_VCOUNT < MODE3_HEIGHT);
}