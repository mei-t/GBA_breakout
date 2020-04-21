#include <stdbool.h>
#include "game.h"
#include "graphics.h"

/* 16-bit volatile register */
#define VR16(x) *((volatile unsigned short *)(x))

#define REG_IOBASE 0x04000000
/* Vertical sync */
#define REG_VCOUNT VR16(REG_IOBASE + 0x6)

/* Status of buttons */
#define BUTTON_RIGHT (1 << 4)
#define BUTTON_LEFT (1 << 5)

/* Wait for vertical sync */
void wait_vsync() {
    while (REG_VCOUNT >= MODE3_HEIGHT);
    while (REG_VCOUNT < MODE3_HEIGHT);
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

    struct game_status state;
    gfx_init(&state);

    // Wait forever
    while(1){
        if(state.ball.y == MODE3_HEIGHT-2){
            gfx_draw_game_over();
            break;
        }else if(state.score == SIDEWAYS_BLOCKS * LENGTHWAYS_BLOCKS){
            // set_pixel(25, 25, 0x7C00); // brue
            gfx_draw_game_clear();
            break;
        }

        char buttons = ioreg[0x130];

        define_ball_orbit(&state);
        update_ball(&state.ball);
        gfx_update_ball(&state.ball);

        // if Right is pressed
        if (is_pressed(BUTTON_RIGHT, buttons) && state.pad.x < GAME_WIDTH - PAD_LENGTH/2 - 1) {
            state.pad.x++;
            gfx_update_pad(&state.pad);
        }

        // if Left is pressed
        if (is_pressed(BUTTON_LEFT, buttons) && state.pad.x > PAD_LENGTH/2 + 1) {
            state.pad.x--;
            gfx_update_pad(&state.pad);
        }

        // vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}