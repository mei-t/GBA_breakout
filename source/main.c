#include <stdbool.h>
#include "game.h"
#include "graphics.h"

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
        gfx_wait_end_frame();
    }

    return 0;
}