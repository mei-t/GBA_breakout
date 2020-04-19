#include "graphics.h"

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

void set_pixel(unsigned short x, unsigned short y, unsigned short color){
    VRAM[y * MODE3_WIDTH + x] = color;
    return;
}

void draw_block(unsigned short x, unsigned short y, unsigned short color){
    for(int i=0; i<BLOCK_HEIGHT; i++){
        for(int j=0; j<BLOCK_LENGTH; j++){
            set_pixel(x+j, y+i, color);
        }
    }
}

void draw_line(unsigned short x, unsigned short y, bool is_white, bool is_sideway){
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

void draw_score(int score){
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

void gfx_init(struct game_state* state){
    for(int i=0; i<MODE3_HEIGHT-1; i++){
        set_pixel(0, i, 0xFFFF);
        set_pixel(GAME_WIDTH, i, 0xFFFF);
    }
    for(int i=0; i<GAME_WIDTH+1; i++){
        set_pixel(i, 0, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT-1, 0xFFFF);
    }

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

    // Draw blocks.
    for(int i=0; i<SIDEWAYS_BLOCKS; i++){
        for(int j=0; j<LENGTHWAYS_BLOCKS; j++){
            // if(i == 1 && j == 0){
            //     set_pixel(2, 2, 0x03E0);
            // }
            // draw_block((i + 1) *(BLOCK_LENGTH+5)-1, (j + 1)*(BLOCK_HEIGHT+5), 0x03E0);
            if(state->block[i][j]){
                draw_block((i + 1) *(BLOCK_LENGTH+5)-1, (j + 1)*(BLOCK_HEIGHT+5), 0x7C00);
                // if(i == 1 && j == 0){
                //     set_pixel(1, 1, 0x001F);
                // }
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

    draw_score(state->score);

    set_pixel(state->ball.x, state->ball.y, 0xFFFF);
}

void delete_block(unsigned short x, unsigned short y, unsigned short color){
    if(VRAM[y * MODE3_WIDTH + x] != color){
        return;
    }
    set_pixel(x, y, 0x0);
    delete_block(x-1, y, color);
    delete_block(x, y-1, color);
    delete_block(x+1, y, color);
    delete_block(x, y+1, color);
}

void gfx_draw_game_over(){
    for(int i=1; i<GAME_WIDTH; i++){
        set_pixel(i, MODE3_HEIGHT/2 - 7, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT/2 - 6, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT/2 + 7, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT/2 + 8, 0xFFFF);
    }
    for(int i=1; i<45; i++){
        for(int j=0; j<12; j++){
            set_pixel(i, MODE3_HEIGHT/2 - 5 + j, 0xFFFF);
            set_pixel(160 - i, MODE3_HEIGHT/2 - 5 + j, 0xFFFF);
        }
    }
    int start_pos[8] = {45, 56, 67, 78, 84, 95, 106, 117};
}

void gfx_draw_game_clear(){}