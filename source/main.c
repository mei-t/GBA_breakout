#include <stdbool.h>
#include "game.h"
#include "graphics.h"

/* 16-bit volatile register */
#define VR16(x) *((volatile unsigned short *)(x))

#define REG_IOBASE 0x04000000
/* Vertical sync */
#define REG_VCOUNT VR16(REG_IOBASE + 0x6)

/* Wait for vertical sync */
void wait_vsync() {
    while (REG_VCOUNT >= MODE3_HEIGHT);
    while (REG_VCOUNT < MODE3_HEIGHT);
}

int main(void){
    struct game_status state;
    game_init(&state);
    gfx_init(&state);

    // Wait forever
    while(1){
        if(state.ball.y == MODE3_HEIGHT-2){
            gfx_draw_game_over();
            return 0;
        }else if(state.score == SIDEWAYS_BLOCKS * LENGTHWAYS_BLOCKS){
            // set_pixel(25, 25, 0x7C00); // brue
            gfx_draw_game_clear();
            return 0;
        }

        game_update(&state);
        game_handle_input(&state);

        // vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}