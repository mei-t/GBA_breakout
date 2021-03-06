#ifndef _CODE_GRAPHICS_H_
#define _CODE_GRAPHICS_H_

/* Size in pixels of screen in mode 3 */
#define MODE3_WIDTH 240
#define MODE3_HEIGHT 160

/* Video memory */
#define VRAM ((volatile unsigned short *)0x06000000)

/* Size of game screen */
#define GAME_WIDTH 160

/* Size of a block */
#define BLOCK_LENGTH 10
#define BLOCK_HEIGHT 8

/* Margin between blocks and game screen */
#define BLOCK_MARGIN_X 8
#define BLOCK_MARGIN_Y 8

/* Gap between blocks */
#define BLOCK_GAP 5

/* Length of the pad */
#define PAD_LENGTH 35
/* Height of the pad */
#define PAD_HEIGHT 140

struct game_status;
struct ball_status;
struct pad_status;

/* Draw the initial screen */
void gfx_init(const struct game_status* state);

/* Erase a block */
void gfx_delete_block(unsigned short x, unsigned short y);

/* Erase the ball from its old position and draw in the new one */
void gfx_update_ball(const struct ball_status* ball_state);

/* Erase the pad from its old position and draw in the new one */
void gfx_update_pad(const struct pad_status* pad_state);

/* Draw the new score */
void gfx_update_score(unsigned int score);

/* Draw the game over screen */
void gfx_draw_game_over();

/* Draw the game clear screen */
void gfx_draw_game_clear();

/* Wait for vertical sync */
void gfx_wait_end_frame();

#endif