#ifndef GFX_H
#define GFX_H

#define INPUT_UP	0
#define INPUT_DOWN	1
#define INPUT_LEFT	2
#define INPUT_RIGHT	3

#include "engine.h"
#include <ncurses.h>
#include <termios.h>

struct gfx_state;

/* Initialization of a graphics context */
struct gfx_state* gfx_init(struct gamestate *);

/* Drawing of a game_state onto a graphics context */
void gfx_draw(struct gfx_state *, struct gamestate *);

/* Blocking get character. Should not be buffered for best results */
int gfx_getch(struct gfx_state *);

/* Destruction of a graphics context */
void gfx_destroy(struct gfx_state *);

/* Sleep for a specifed millisecond period */
void gfx_sleep(int ms);

void draw_then_sleep(struct gfx_state *, struct gamestate *);

#ifdef NCURSES
struct gfx_state {
    WINDOW *window;
    size_t window_height, window_width;
};
#else 
struct gfx_state {
    struct termios oldt, newt;
};
#endif

#endif
