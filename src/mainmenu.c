#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <libintl.h>

#define _(STR) gettext(STR)
#define N_(STR) STR

#include "mainmenu.h"

enum {
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

        diff_y = 2,

        std_bg   = COLOR_BLACK,
        std_fg   = COLOR_WHITE,
        std_attr = 0,

        name_fg = 0,
        name_bg,
        name_attr,
        body_fg,
        body_bg,
        body_attr,
        cur_fg,
        cur_bg,
        cur_attr,
        apar_fg,
        apar_bg,
        apar_attr,

        name_pair = 1,
        body_pair,
        cur_pair,
        apar_pair,

        sr_params_count = 5,
        range_default   = 1,
        range_diff,
        range_min,
        range_max,

        param_border = '"',
};

struct cur {
        int cur_x, cur_y, pos;
};

static int settings_pos, exit_pos;
static WINDOW *name, *pad;

static char fileloc[mm_bufsize];

extern const char fn[], pn[], mt[][mm_bufsize],
                              st[][mm_bufsize],
                              sr[][mm_bufsize];
extern       void *sp[];
extern const int  mc, sc, mm_colors[];

static int is_float(int n)
{
        if (0 == strcmp(sr[n * sr_params_count], "f"))
                return 1;
        return 0;
}

static void init_fileloc()
{
        strncpy(fileloc, getenv("HOME"), mm_bufsize);
        strncat(fileloc, "/", mm_bufsize-1);
        strncat(fileloc, fn, mm_bufsize-1);
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
        if (has_colors())
                start_color();
        init_pair(name_pair, mm_colors[name_fg], mm_colors[name_bg]);
        init_pair(body_pair, mm_colors[body_fg], mm_colors[body_bg]);
        init_pair(cur_pair,  mm_colors[cur_fg],  mm_colors[cur_bg]);
        init_pair(apar_pair, mm_colors[apar_fg], mm_colors[apar_bg]);
}

static void load_defaults()
{
        int i;
        for (i = 0; i < sc; i++)
                if (is_float(i))
                        *(double *)sp[i] = atof(sr[sr_params_count *
                                                i + range_default]);
                else
                        *(int *)sp[i] = atoi(sr[sr_params_count *
                                                i + range_default]);
}

static void parse_param(FILE *f, char *param, int bufsize)
{
        int i, c;
        do
                c = fgetc(f);
        while (c != param_border);
        for (i = 0; i < bufsize-1; i++) {
                c = fgetc(f);
                if (c == param_border || c == EOF)
                        break;
                param[i] = c;
        }
        param[i] = 0;
}

static int load_params()
{
        int i;
        char param[mm_bufsize];
        FILE *f;
        f = fopen(fileloc, "r");
        if (!f)
                return 0;
        for (i = 0; i < sc; i++) {
                parse_param(f, param, mm_bufsize);
                if (is_float(i))
                        *(double *)sp[i] = atof(param);
                else
                        *(int *)sp[i] = atoi(param);
        }
        fclose(f);
        return 1;
}

static void save_params()
{
        int i;
        FILE *f;
        f = fopen(fileloc, "w");
        if (!f)
                return;
        for (i = 0; i < sc; i++) {
                if (is_float(i))
                        fprintf(f, "%c%lf%c\n", param_border,
                                                *(double *)sp[i],
                                                param_border);
                else
                        fprintf(f, "%c%d%c\n", param_border,
                                               *(int *)sp[i],
                                               param_border);
        }
        fclose(f);
}

static void initmm()
{
        int res;
        settings_pos = mc - 2;
        exit_pos     = mc - 1;
        init_fileloc();
        res = load_params();
        if (!res) {
                load_defaults();
                save_params();
        }
        initcurses();
}

static void freemm()
{
        delwin(name);
        delwin(pad);
}

static void show_cur(struct cur c)
{
        wattrset(pad, COLOR_PAIR(cur_pair) | mm_colors[cur_attr]);
        mvwaddch(pad, c.cur_y, c.cur_x, cur_symb);
}

static void hide_cur(struct cur c)
{
        wattrset(pad, A_NORMAL);
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

static void show_param(int n, int cur)
{
        int i, x, y, col;
        col = getmaxx(stdscr);
        x = body_x + strlen(_(st[n])) + 1;
        y = body_y + n * 2;
        if (cur)
                wattrset(pad, COLOR_PAIR(apar_pair) | mm_colors[apar_attr]);
        else
                wattrset(pad, COLOR_PAIR(body_pair) | mm_colors[body_attr]);
        if (is_float(n))
                mvwprintw(pad, y, x, "<%f>", *(double *)sp[n]);
        else
                mvwprintw(pad, y, x, "<%d>", *(int *)sp[n]);
        wattrset(pad, A_NORMAL);
        for (i = getcurx(pad); i < col; i++)
                waddch(pad, ' ');
}

static void get_range_params(int n, void *def, void *diff, void *min, void *max)
{
        if (is_float(n)) {
                *(double *)def  = atof(sr[sr_params_count*n + range_default]);
                *(double *)diff = atof(sr[sr_params_count*n + range_diff]);
                *(double *)min  = atof(sr[sr_params_count*n + range_min]);
                *(double *)max  = atof(sr[sr_params_count*n + range_max]);
        } else {
                *(int *)def  = atoi(sr[sr_params_count*n + range_default]);
                *(int *)diff = atoi(sr[sr_params_count*n + range_diff]);
                *(int *)min  = atoi(sr[sr_params_count*n + range_min]);
                *(int *)max  = atoi(sr[sr_params_count*n + range_max]);
        }
}

static void decrease_param(int n)
{
        int idef, idiff, imin, imax;
        double fdef, fdiff, fmin, fmax;
        if (is_float(n)) {
                get_range_params(n, &fdef, &fdiff, &fmin, &fmax);
                *(double *)sp[n] -= fdiff;
                if (*(double *)sp[n] < fmin)
                        *(double *)sp[n] = fmax;
        } else {
                get_range_params(n, &idef, &idiff, &imin, &imax);
                *(int *)sp[n] -= idiff;
                if (*(int *)sp[n] < imin)
                        *(int *)sp[n] = imax;
        }
        save_params();
} 

static void increase_param(int n)
{
        int idef, idiff, imin, imax;
        double fdef, fdiff, fmin, fmax;
        if (is_float(n)) {
                get_range_params(n, &fdef, &fdiff, &fmin, &fmax);
                *(double *)sp[n] += fdiff;
                if (*(double *)sp[n] > fmax)
                        *(double *)sp[n] = fmin;
        } else {
                get_range_params(n, &idef, &idiff, &imin, &imax);
                *(int *)sp[n] += idiff;
                if (*(int *)sp[n] > imax)
                        *(int *)sp[n] = imin;
        }
        save_params();
} 

static void draw_name()
{
        int col = getmaxx(stdscr);
        name = newwin(name_nrow, col, name_sy, name_sx);
        wattrset(name, COLOR_PAIR(name_pair) | mm_colors[name_attr]);
        mvwaddstr(name, name_y, (col - strlen(pn)) / 2, _(pn));
        wrefresh(name);
}

static void draw_mm(int padpos, struct cur c)
{
        int i, row, col, y = body_y, x = body_x;
        getmaxyx(stdscr, row, col);
        wclear(pad);
        wattrset(pad, COLOR_PAIR(body_pair) | mm_colors[body_attr]);
        for (i = 0; i < mc; i++, y += diff_y)
                mvwaddstr(pad, y, x, _(mt[i]));
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
                wattrset(pad, COLOR_PAIR(body_pair) | mm_colors[body_attr]);
                mvwaddstr(pad, y, x, _(st[i]));
                i == 0 ? show_param(i, 1) : show_param(i, 0);
        }
        wattrset(pad, COLOR_PAIR(body_pair) | mm_colors[body_attr]);
        mvwaddstr(pad, y, x, _(mt[exit_pos]));
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
                                if (c.pos != exit_pos)
                                        show_param(c.pos, 0);
                                move_cur(&c, -1);
                                show_param(c.pos, 1);
                                padpos -= diff_y;
                        }
                        break;
                case KEY_DOWN:
                        if (c.pos < sc) {
                                show_param(c.pos, 0);
                                move_cur(&c, 1);
                                if (c.pos != exit_pos)
                                        show_param(c.pos, 1);
                                padpos += diff_y;
                        }
                        break;
                case KEY_LEFT:
                        if (c.pos == sc)
                                break;
                        decrease_param(c.pos);
                        show_param(c.pos, 1);
                        break;
                case KEY_RIGHT:
                        if (c.pos == sc)
                                break;
                        increase_param(c.pos);
                        show_param(c.pos, 1);
                        break;
                }
                prefresh(pad, padpos, 0, body_sy, body_sx, row-name_nrow, col);
        }
}

int mainmenu()
{
        int res;
        initmm();
        draw_name();
        for (;;) {
                res = handle_mm();
                if (res == settings_choise)
                        handle_sm();
                else
                        break;
        }
        freemm();
        return res;
}
