#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "merge.h"
#include "gfx.h"
#include <libintl.h>
#include <locale.h>

#define iterate(n, expression)\
    do {\
        int i;\
        for (i = 0; i < n; ++i) { expression; }\
    } while (0)

void draw_then_sleep(struct gfx_state *s, struct gamestate *g)
{
    gfx_draw(s, g);
    /* Have a fixed time for each turn to animate (160 default) */
    gfx_sleep(160 / g->opts->grid_width);
}

struct gfx_state* gfx_init(struct gamestate *g)
{
    (void) g;

    struct gfx_state *s = malloc(sizeof(struct gfx_state));
    if (!s) return NULL;
    tcgetattr(STDIN_FILENO, &s->oldt);
    s->newt = s->oldt;
    s->newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &s->newt);
    return s;
}

void gfx_draw(struct gfx_state *s, struct gamestate *g)
{
    (void) s;

#ifdef VT100
    printf("\033[2J\033[H");
#endif

    if (g->score_last)
        printf(gettext("Score: %ld (+%ld)\n"), g->score, g->score_last);
    else
        printf(gettext("Score: %ld\n"), g->score);

    if (g->score >= g->score_high)
        g->score_high = g->score;

    printf(gettext("   Hi: %ld\n"), g->score_high);

    iterate((g->print_width + 2) * g->opts->grid_width + 1, printf("-")); printf("\n");

    int x, y;
    for (y = 0; y < g->opts->grid_width; ++y) {
        printf("|");

        for (x = 0; x < g->opts->grid_width; ++x) {
            if (g->grid[x][y])
                printf("%*zd |", g->print_width, merge_value(g->grid[x][y]));
            else
                printf("%*s |", g->print_width, "");
        }
        printf("\n");
    }

    iterate((g->print_width + 2) * g->opts->grid_width + 1, printf("-")); printf("\n\n");
}

int gfx_getch(struct gfx_state *s)
{
    (void) s;
    return getchar();
}

void gfx_sleep(int ms)
{
    usleep(ms * 1000);
}

void gfx_destroy(struct gfx_state *s)
{
    tcsetattr(STDIN_FILENO, TCSANOW, &s->oldt);
    free(s);
}
