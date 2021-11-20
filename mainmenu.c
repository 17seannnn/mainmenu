#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "mainmenu.h"

static const char *pn;
static const char (*mt)[mm_bufsize], (*st)[mm_bufsize], (*sr)[mm_bufsize];
static const void **sp;
static int mc, sc;

enum {
        sr_params_count = 4,
        settings_choise = 254,
        cur_symb = '>'
};

struct cur {
        int cur_x, cur_y, pos;
};

static void initcurses()
{
        initscr();
        noecho();
        cbreak();
        keypad(stdscr, 1);
        curs_set(0);
}

void initmainmenu(const char  *program_name,
                  const char (*mainmenu_text)[mm_bufsize],
                  const char (*settings_text)[mm_bufsize],
                  const char (*settings_range)[mm_bufsize],
                  const void  *settings_pointer[],
                  const int    mainmenu_count,
                  const int    settings_count)
{
        pn = program_name;
        mt = mainmenu_text;
        st = settings_text;
        sr = settings_range;
        sp = settings_pointer;
        mc = mainmenu_count;
        sc = settings_count;
}

static int is_float(int n)
{
        if (0 == strcmp(sr[n * sr_params_count], "f"))
                return 1;
        return 0;
}

static void show_cur(struct cur c)
{
        mvaddch(c.cur_y, c.cur_x, cur_symb);
        refresh();
}

static void hide_cur(struct cur c)
{
        mvaddch(c.cur_y, c.cur_x, ' ');
        refresh();
}

static void move_cur(struct cur *c, int dpos)
{
        hide_cur(*c);
        if (dpos > 0)
                c->cur_y += 2;
        else
                c->cur_y -= 2;
        c->pos += dpos;
        show_cur(*c);
}

void getmenuxy(const char (*p)[mm_bufsize], size_t n, int *x, int *y)
{
        int i, max = 0;
        for (i = 0; i < n; i++)
                if (strlen(p[i]) > strlen(p[max]))
                        max = i;
        getmaxyx(stdscr, *y, *x);
        *y = (*y - mc * 2) / 2;
        *x = (*x - strlen(mt[max])) / 2;
}

static void draw_mm()
{
        int i, x, y;
        clear();
        getmaxyx(stdscr, y, x);
        mvaddstr(y/8, (x - strlen(pn)) / 2, pn);
        getmenuxy(mt, mc, &x, &y);
        for (i = 0; i < mc; i++, y += 2)
                mvaddstr(y, x, mt[i]);
        refresh();
}

static int handle_mm()
{
        int key;
        struct cur c;
        getmenuxy(mt, mc, &c.cur_x, &c.cur_y);
        c.cur_x -= 2;
        c.pos = 0;
        show_cur(c);
        while ((key = getch()) != '\n') {
                switch (key) {
                case KEY_UP:
                        if (c.pos > 0)
                                move_cur(&c, -1);
                        break;
                case KEY_DOWN:
                        if (c.pos < mc-1)
                                move_cur(&c, 1);
                        break;
                }
        }
        if (c.pos == mc-1)
                return exit_choise;
        else
        if (c.pos == mc-2)
                return settings_choise;
        else
                return c.pos;
}

static void show_param(int n)
{
        int x, y;
        getmenuxy(st, sc, &x, &y);
        x += strlen(st[n]) + 1;
        y += n * 2;
        if (is_float(n))
                mvprintw(y, x, "<%f>      ", *(double *)sp[n]);
        else
                mvprintw(y, x, "<%d>      ", *(int *)sp[n]);
}

static void decrease_param(int n)
{
        int irange, imin, imax;
        double frange, fmin, fmax;
        if (is_float(n)) {
                sscanf(sr[sr_params_count*n + 1], "%lf", &frange);
                sscanf(sr[sr_params_count*n + 2], "%lf", &fmin);
                sscanf(sr[sr_params_count*n + 3], "%lf", &fmax);
                *(double *)sp[n] -= frange;
                if (*(double *)sp[n] < fmin)
                        *(double *)sp[n] = fmax;
        } else {
                sscanf(sr[sr_params_count*n + 1], "%d", &irange);
                sscanf(sr[sr_params_count*n + 2], "%d", &imin);
                sscanf(sr[sr_params_count*n + 3], "%d", &imax);
                *(int *)sp[n] -= irange;
                if (*(int *)sp[n] < imin)
                        *(int *)sp[n] = imax;
        }
} 

static void increase_param(int n)
{
        int irange, imin, imax;
        double frange, fmin, fmax;
        if (is_float(n)) {
                sscanf(sr[sr_params_count*n + 1], "%lf", &frange);
                sscanf(sr[sr_params_count*n + 2], "%lf", &fmin);
                sscanf(sr[sr_params_count*n + 3], "%lf", &fmax);
                *(double *)sp[n] += frange;
                if (*(double *)sp[n] > fmax)
                        *(double *)sp[n] = fmin;
        } else {
                sscanf(sr[sr_params_count*n + 1], "%d", &irange);
                sscanf(sr[sr_params_count*n + 2], "%d", &imin);
                sscanf(sr[sr_params_count*n + 3], "%d", &imax);
                *(int *)sp[n] += irange;
                if (*(int *)sp[n] > imax)
                        *(int *)sp[n] = imin;
        }
} 

static void draw_sm()
{
        int i, x, y;
        clear();
        getmenuxy(st, sc, &x, &y);
        for (i = 0; i < sc; i++, y += 2) {
                mvaddstr(y, x, st[i]);
                show_param(i);
        }
        mvaddstr(y, x, mt[mc-1]);
        refresh();
}

static void handle_sm()
{
        int key;
        struct cur c;
        getmenuxy(st, sc, &c.cur_x, &c.cur_y);
        c.cur_x -= 2;
        c.pos = 0;
        show_cur(c);
        while ((key = getch()) != '\n') {
                switch (key) {
                case KEY_UP:
                        if (c.pos > 0)
                                move_cur(&c, -1);
                        break;
                case KEY_DOWN:
                        if (c.pos < sc)
                                move_cur(&c, 1);
                        break;
                case KEY_LEFT:
                        if (c.pos == sc)
                                break;
                        decrease_param(c.pos);
                        show_param(c.pos);
                        break;
                case KEY_RIGHT:
                        if (c.pos == sc)
                                break;
                        increase_param(c.pos);
                        show_param(c.pos);
                        break;
                }
        }
}

int mainmenu()
{
        int res;
        initcurses();
        timeout(-1);
        for (;;) {
                draw_mm();
                res = handle_mm();
                if (res != settings_choise)
                        break;
                draw_sm();
                handle_sm();
        }
        endwin();
        return res;
}
