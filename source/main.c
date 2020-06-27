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
        game_handle_input(&state);
        switch(game_update(&state)) {
            case PLAYING:
                break;
            case GAME_WON:
                gfx_draw_game_clear();
                return 0;
            case GAME_OVER:
                gfx_draw_game_over();
                return 0;
        }

        // vram[80*240 + x] = 0xFFFF; // X = x, Y = 80, C = 111111111111 = W
        wait_vsync();
    }

    return 0;
}