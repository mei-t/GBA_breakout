#include <stdbool.h>

/* 16-bit volatile register */
#define VR16(x) *((volatile unsigned short *)(x))

#define REG_IOBASE 0x04000000
/* Vertical sync */
#define REG_VCOUNT VR16(REG_IOBASE + 0x6)

/* Size in pixels of screen in mode 3 */
#define MODE3_WIDTH 240
#define MODE3_HEIGHT 160

/* Status of buttons */
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)

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
#define SIDEWAYS_BLOCKS 5
#define LENGTHWAYS_BLOCKS 2

/* Wait for vertical sync */
void wait_vsync() {
    while (REG_VCOUNT >= MODE3_HEIGHT);
    while (REG_VCOUNT < MODE3_HEIGHT);
}

/* If the button pressed or not. */
bool is_pressed(unsigned short BUTTON, unsigned short buttons){
    return (BUTTON & buttons) == 0;
}

/* Set a pixel. */
void set_pixel(unsigned short x, unsigned short y, unsigned short color){
    VRAM[y * MODE3_WIDTH + x] = color;
    return;
}

/* Draw a block. */
void draw_block(unsigned short x, unsigned short y, unsigned short color){
    for(int i=0; i<BLOCK_HEIGHT; i++){
        for(int j=0; j<BLOCK_LENGTH; j++){
            set_pixel(x+j, y+i, color);
        }
    }
}

/* Draw score on screen. */
void draw_score(int score){}

/* Draw default screen. */
void init(int *score, int pad_x){
    for(int i=0; i<MODE3_HEIGHT-1; i++){
        set_pixel(0, i, 0xFFFF);
        set_pixel(GAME_WIDTH, i, 0xFFFF);
    }
    for(int i=0; i<GAME_WIDTH+1; i++){
        set_pixel(i, 0, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT-1, 0xFFFF);
    }

    // Draw blocks.
    for(int i=1; i<=SIDEWAYS_BLOCKS; i++){
        for(int j=1; j<=LENGTHWAYS_BLOCKS; j++){
            draw_block(i*(BLOCK_LENGTH+5)-1, j*(BLOCK_HEIGHT+5), 0x7C00);
        }
    }

    // Draw a pad.
    for(int i=0; i<PAD_LENGTH; i++){
        set_pixel(pad_x-PAD_LENGTH/2+i, PAD_HEIGHT, 0xFFFF);
    }

    draw_score(*score);
}

struct ball_status{
    unsigned short x;
    unsigned short y;
    bool is_up;
    bool is_left;
};

/* Delete the block which the ball hit. */
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

/* Draw "GAME OVER" on screen. */
void draw_game_over(){}

/* Draw "GAME CLEAR" on screen. */
void draw_game_clear(){}

int main(void){
    volatile char *ioreg = (char *)0x04000000;
    ioreg[0] = 0x03; // not understand
    ioreg[1] = 0x04; // not understand

    // Write pixel colors into VRAM
    // volatile unsigned short *vram = (unsigned short *)0x06000000;
    // vram[80*240 + 115] = 0x001F; // X = 115, Y = 80, C = 000000000011111 = R
    // vram[80*240 + 120] = 0x03E0; // X = 120, Y = 80, C = 000001111100000 = G
    // vram[80*240 + 125] = 0x7C00; // X = 125, Y = 80, C = 111110000000000 = B

    int score = 0;
    int pad_x = GAME_WIDTH/2;
    init(&score, pad_x);
    
    struct ball_status bs = {GAME_WIDTH/2, MODE3_HEIGHT/2, true, false};
    set_pixel(bs.x, bs.y, 0xFFFF);

    // Wait forever
    while(1){
        if(bs.y == MODE3_HEIGHT-2){
            draw_game_over();
            break;
        }else if(score == SIDEWAYS_BLOCKS * LENGTHWAYS_BLOCKS){
            // set_pixel(25, 25, 0x7C00); // brue
            draw_game_clear();
            break;
        }

        char buttons = ioreg[0x130];

        define_ball_orbit(&bs, &score);
        set_pixel(bs.x, bs.y, 0x0);
        bs.is_up ? bs.y-- : bs.y++;
        bs.is_left ? bs.x-- : bs.x++;
        set_pixel(bs.x, bs.y, 0xFFFF);

        // if Right is pressed
        if (is_pressed(BUTTON_RIGHT, buttons) && pad_x < GAME_WIDTH - PAD_LENGTH/2 - 1) {
            set_pixel(pad_x-PAD_LENGTH/2, PAD_HEIGHT, 0x0);
            pad_x++;
            set_pixel(pad_x+PAD_LENGTH/2, PAD_HEIGHT, 0xFFFF);
        }

        // if Left is pressed
        if (is_pressed(BUTTON_LEFT, buttons) && pad_x > PAD_LENGTH/2 + 1) {
            set_pixel(pad_x+PAD_LENGTH/2, PAD_HEIGHT, 0x0);
            pad_x--;
            set_pixel(pad_x-PAD_LENGTH/2, PAD_HEIGHT, 0xFFFF);
        }

        // vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}