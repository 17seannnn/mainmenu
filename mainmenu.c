#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "mainmenu.h"

static WINDOW *pad;

static const char *pn;
static const char (*mt)[mm_bufsize], (*st)[mm_bufsize], (*sr)[mm_bufsize];
static const void **sp;
static int mc, sc;

static int settings_pos, exit_pos;

enum {
        sr_params_count = 4,
        settings_choise = 254,
        cur_symb        = '>',

        name_nrow = 3,
        name_y    = 1,
        name_sx   = 0,
        name_sy   = 0,

        body_x  = 20,
        body_y  = 6,
        body_sx = 0,
        body_sy = name_nrow,

        diff_y = 2
};

struct cur {
        int cur_x, cur_y, pos;
};

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
        settings_pos = mc - 2;
        exit_pos     = mc - 1;
}

static void initcurses()
{
        int row, col;
        getmaxyx(stdscr, row, col);
        pad = newpad(row - name_nrow, col);
        keypad(pad, 1);
        noecho();
        cbreak();
        curs_set(0);
        timeout(-1);
}

static void show_cur(struct cur c)
{
        mvwaddch(pad, c.cur_y, c.cur_x, cur_symb);
}

static void hide_cur(struct cur c)
{
        mvwaddch(pad, c.cur_y, c.cur_x, ' ');
}

static void move_cur(struct cur *c, int dpos)
{
        hide_cur(*c);
        if (dpos > 0)
                c->cur_y += diff_y;
        else
                c->cur_y -= diff_y;
        c->pos += dpos;
        show_cur(*c);
}

static int is_float(int n)
{
        if (0 == strcmp(sr[n * sr_params_count], "f"))
                return 1;
        return 0;
}

static void show_param(int n)
{
        int x, y;
        x = body_x + strlen(st[n]) + 1;
        y = body_y + n * 2;
        if (is_float(n))
                mvwprintw(pad, y, x, "<%f>         ", *(double *)sp[n]);
        else
                mvwprintw(pad, y, x, "<%d>         ", *(int *)sp[n]);
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

static void draw_name(WINDOW **name)
{
        int col = getmaxx(stdscr);
        *name = newwin(name_nrow, col, name_sy, name_sx);
        mvwaddstr(*name, name_y, (col - strlen(pn)) / 2, pn);
        wrefresh(*name);
}

static void draw_mm(int padpos, struct cur c)
{
        int i, row, col, y = body_y, x = body_x;
        getmaxyx(stdscr, row, col);
        wclear(pad);
        for (i = 0; i < mc; i++, y += diff_y)
                mvwaddstr(pad, y, x, mt[i]);
        show_cur(c);
        prefresh(pad, padpos, 0, body_sy, body_sx, row-name_nrow, col);
}

static int handle_mm()
{
        int row, col, key, padpos = 0;
        struct cur c = { body_x - 2, body_y, 0 };
        getmaxyx(stdscr, row, col);
        draw_mm(padpos, c);
        while ((key = wgetch(pad)) != '\n') {
                switch (key) {
                case KEY_UP:
                        if (c.pos > 0) {
                                move_cur(&c, -1);
                                padpos -= diff_y;
                        }
                        break;
                case KEY_DOWN:
                        if (c.pos < exit_pos) {
                                move_cur(&c, 1);
                                padpos += diff_y;
                        }
                        break;
                }
                prefresh(pad, padpos, 0, body_sy, body_sx, row-name_nrow, col);
        }
        if (c.pos == settings_pos)
                return settings_choise;
        else
        if (c.pos == exit_pos)
                return exit_choise;
        else
                return c.pos;
}

static void draw_sm(int padpos, struct cur c)
{
        int i, row, col, y = body_y, x = body_x;
        getmaxyx(stdscr, row, col);
        wclear(pad);
        for (i = 0; i < sc; i++, y += 2) {
                mvwaddstr(pad, y, x, st[i]);
                show_param(i);
        }
        mvwaddstr(pad, y, x, mt[exit_pos]);
        show_cur(c);
        prefresh(pad, padpos, 0, body_sy, body_sx, row-name_nrow, col);
}

static void handle_sm()
{
        int key, row, col, padpos = 0;
        struct cur c = { body_x - 2, body_y, 0 };
        getmaxyx(stdscr, row, col);
        draw_sm(padpos, c);
        while ((key = wgetch(pad)) != '\n') {
                switch (key) {
                case KEY_UP:
                        if (c.pos > 0) {
                                move_cur(&c, -1);
                                padpos -= diff_y;
                        }
                        break;
                case KEY_DOWN:
                        if (c.pos < sc) {
                                move_cur(&c, 1);
                                padpos += diff_y;
                        }
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
                prefresh(pad, padpos, 0, body_sy, body_sx, row-name_nrow, col);
        }
}

int mainmenu()
{
        int res;
        WINDOW *name;
        initcurses();
        draw_name(&name);
        for (;;) {
                res = handle_mm();
                if (res == settings_choise)
                        handle_sm();
                else
                        break;
        }
        delwin(name);
        delwin(pad);
        return res;
}
