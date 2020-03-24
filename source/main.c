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

/* Size in pixels of screen in mode 3 */
#define MODE3_WIDTH 240
#define MODE3_HEIGHT 160

/* Size of game screen */
#define GAME_WIDTH 160
/* Size of a block */
#define BLOCK_LENGTH 4
#define BLOCK_HEIGHT 3
/* Length of the pad */
#define PAD_LENGTH 15
/* Height of the pad */
#define PAD_HEIGHT 140

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

/* Draw default screen. */
void init(){
    for(int i=0; i<MODE3_HEIGHT-1; i++){
        set_pixel(0, i, 0xFFFF);
        set_pixel(GAME_WIDTH, i, 0xFFFF);
    }
    for(int i=0; i<GAME_WIDTH+1; i++){
        set_pixel(i, 0, 0xFFFF);
        set_pixel(i, MODE3_HEIGHT-1, 0xFFFF);
    }

    // Draw blocks.
    for(int i=1; i<21; i++){
        for(int j=1; j<6; j++){
            draw_block(i*5-1, j*4, 0x7C00);
        }
    }
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
    set_pixel(x, y, color);
    delete_block(x-1, y, color);
    delete_block(x, y-1, color);
    delete_block(x+1, y, color);
    delete_block(x, y+1, color);
}

bool can_go_horizontal(unsigned short x, unsigned short y, bool is_left){
    is_left ? x-- : x++;
    unsigned short next_color = VRAM[y*MODE3_WIDTH + x];
    if(next_color == 0x0){
        return false;
    }
    if(next_color != 0xFFFF){
        delete_block(x, y, next_color);
    }
    return true;
}
bool can_go_vertical(unsigned short x, unsigned short y, bool is_up){
    is_up ? y-- : y++;
    unsigned short next_color = VRAM[y*MODE3_WIDTH + x];
    if(next_color == 0x0){
        return false;
    }
    if(next_color != 0xFFFF){
        delete_block(x, y, next_color);
    }
    return true;
}
void check_next(struct ball_status *bs){
    unsigned short next_x = bs->x;
    unsigned short next_y = bs->y;
    bs->is_up ? next_y-- : next_y++;
    bs->is_left ? next_x-- : next_x++;
    unsigned short next_color = VRAM[next_y * MODE3_WIDTH + next_x];
    if(next_color == 0x0){
        return;
    }
    if(next_color != 0xFFFF){
        delete_block(next_x, next_y, next_color);
    }
    bs->is_left = !bs->is_left;
    bs->is_up = !bs->is_up;
}

void define_ball_orbit(struct ball_status *bs){ // Reference is invalid. Why?
    bool is_straight = true;
    if(can_go_horizontal(bs->x, bs->y, bs->is_left)){
        bs->is_left = !bs->is_left;
        is_straight = false;
    }
    if(can_go_vertical(bs->x, bs->y, bs->is_up)){
        bs->is_up = !bs->is_up;
        is_straight = false;
    }
    if(is_straight){
        check_next(bs);
    }
}

int main(void){
    volatile char *ioreg = (char *)0x04000000;
    ioreg[0] = 0x03; // not understand
    ioreg[1] = 0x04; // not understand

    // Write pixel colors into VRAM
    // volatile unsigned short *vram = (unsigned short *)0x06000000;
    // vram[80*240 + 115] = 0x001F; // X = 115, Y = 80, C = 000000000011111 = R
    // vram[80*240 + 120] = 0x03E0; // X = 120, Y = 80, C = 000001111100000 = G
    // vram[80*240 + 125] = 0x7C00; // X = 125, Y = 80, C = 111110000000000 = B

    init();

    int x = 120;
    for(int i=0; i<PAD_LENGTH; i++){
        set_pixel(x-PAD_LENGTH/2+i, PAD_HEIGHT, 0xFFFF);
    }

    struct ball_status bs = {GAME_WIDTH/2, MODE3_HEIGHT/2, true, false};
    set_pixel(bs.x, bs.y, 0xFFFF);

    // for(int i=0; i<10; i++){
    //     for(int j=0; j<3; j++){
    //         for(int k=0; k<2; k++){
    //             vram[2*240 + i]
    //         }
    //     }
    // }


    // Wait forever
    while(1){
        char buttons = ioreg[0x130];

        define_ball_orbit(&bs);
        set_pixel(bs.x, bs.y, 0x0);
        bs.is_up ? bs.y-- : bs.y++;
        bs.is_left ? bs.x-- : bs.x++;
        set_pixel(bs.x, bs.y, 0xFFFF);

        // if Right is pressed
        if (is_pressed(BUTTON_RIGHT, buttons) && x < GAME_WIDTH - PAD_LENGTH/2 - 1) {
            set_pixel(x-PAD_LENGTH/2, PAD_HEIGHT, 0x0);
            x++;
            set_pixel(x+PAD_LENGTH/2, PAD_HEIGHT, 0xFFFF);
        }

        // if Left is pressed
        if (is_pressed(BUTTON_LEFT, buttons) && x > PAD_LENGTH/2 + 1) {
            set_pixel(x+PAD_LENGTH/2, PAD_HEIGHT, 0x0);
            x--;
            set_pixel(x-PAD_LENGTH/2, PAD_HEIGHT, 0xFFFF);
        }

        // vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}