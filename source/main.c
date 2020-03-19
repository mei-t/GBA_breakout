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

/* Height of the bar */
#define BAR_HEIGHT 140

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

struct ball_status{
    unsigned short x;
    unsigned short y;
    bool is_up;
    bool is_left;
};


int main(void){
    volatile char *ioreg = (char *)0x04000000;
    ioreg[0] = 0x03; // not understand
    ioreg[1] = 0x04; // not understand

    // Write pixel colors into VRAM
    // volatile unsigned short *vram = (unsigned short *)0x06000000;
    // vram[80*240 + 115] = 0x001F; // X = 115, Y = 80, C = 000000000011111 = R
    // vram[80*240 + 120] = 0x03E0; // X = 120, Y = 80, C = 000001111100000 = G
    // vram[80*240 + 125] = 0x7C00; // X = 125, Y = 80, C = 111110000000000 = B

    int x = 120;
    for(int i=0; i<5; i++){
        set_pixel(x-2+i, BAR_HEIGHT, 0xFFFF);
    }

    struct ball_status bs = {MODE3_WIDTH/2, MODE3_WIDTH, true, false};
    set_pixel(bs.x, bs.y, 0xFFFF);

    for(int i=0; i<2; i++){
        for(int j=0; j<3; j++){
            set_pixel(j, 2+i, 0x7C00);
        }
    }
    // for(int i=0; i<10; i++){
    //     for(int j=0; j<3; j++){
    //         for(int k=0; k<2; k++){
    //             vram[2*240 + i]
    //         }
    //     }
    // }


    // Wait forever
    while(1){
        // unsigned short color = 0x001F;
        char buttons = ioreg[0x130];
        // vram[80*240 + x] = 0x0;

        if((bs.is_up && bs.y == 0) || (!bs.is_up && bs.y == MODE3_HEIGHT-1)){
            bs.is_up = !bs.is_up;
        }
        if((bs.is_left && bs.x == 0) || (!bs.is_left && bs.x == MODE3_WIDTH-1)){
            bs.is_left = !bs.is_left;
        }
        set_pixel(bs.x, bs.y, 0x0);
        // set_pixel(MODE3_WIDTH/2, bs.y, 0x0);
        if(bs.is_up){
            bs.y--;
        }else{
            bs.y++;
        }
        if(bs.is_left){
            bs.x--;
        }else{
            bs.x++;
        }
        set_pixel(bs.x, bs.y, 0xFFFF);
        // set_pixel(MODE3_WIDTH/2, bs.y, 0xFFFF);

        // if Right is pressed
        if (is_pressed(BUTTON_RIGHT, buttons) && x < 237) {
            set_pixel(x-2, BAR_HEIGHT, 0x0);
            x++;
            set_pixel(x+2, BAR_HEIGHT, 0xFFFF);
        }

        // if Left is pressed
        if (is_pressed(BUTTON_LEFT, buttons) && x > 2) {
            set_pixel(x+2, BAR_HEIGHT, 0x0);
            x--;
            set_pixel(x-2, BAR_HEIGHT, 0xFFFF);
        }

        // vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}