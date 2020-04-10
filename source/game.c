#include "game.h"

void hit_block(unsigned short x, unsigned short y, unsigned short color, int *score){
    delete_block(x, y, color);
    (*score)++;
    draw_score(*score);
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

void check_next(struct ball_status *bs, int *score){
    unsigned short next_x = bs->x;
    unsigned short next_y = bs->y;
    bs->is_up ? next_y-- : next_y++;
    bs->is_left ? next_x-- : next_x++;
    unsigned short next_color = VRAM[next_y * MODE3_WIDTH + next_x];
    if(next_color == 0x0){
        return;
    }
    if(next_color != 0xFFFF){
        hit_block(next_x, next_y, next_color, score);
    }
    bs->is_left = !bs->is_left;
    bs->is_up = !bs->is_up;
}

void define_ball_orbit(struct ball_status *bs, int *score){ // Reference is invalid. Why?
    bool is_straight = true;
    if(can_go_horizontal(bs->x, bs->y, bs->is_left, score)){
        bs->is_left = !bs->is_left;
        is_straight = false;
    }
    if(can_go_vertical(bs->x, bs->y, bs->is_up, score)){
        bs->is_up = !bs->is_up;
        is_straight = false;
    }
    if(is_straight){
        check_next(bs, score);
    }
}