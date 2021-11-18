#include <stdio.h>
#include <string.h>
#include <curses.h>

#include "mainmenu.h"

#define MM_COUNT 3

static const char *pn;
static const char (*mt)[mm_bufsize], (*st)[mm_bufsize], (*sr)[mm_bufsize];
static const void **sp;
static int sc;

enum { cur_symb = '>' };

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
                  const int    settings_count)
{
        pn = program_name;
        mt = mainmenu_text;
        st = settings_text;
        sr = settings_range;
        sp = settings_pointer;
        sc = settings_count;
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
        *y = (*y - MM_COUNT * 2) / 2;
        *x = (*x - strlen(mt[max])) / 2;
}

static void draw_mm()
{
        int i, x, y;
        clear();
        getmaxyx(stdscr, y, x);
        mvaddstr(y/8, (x - strlen(pn)) / 2, pn);
        getmenuxy(mt, MM_COUNT, &x, &y);
        for (i = 0; i < MM_COUNT; i++, y += 2)
                mvaddstr(y, x, mt[i]);
        refresh();
}

static int handle_mm()
{
        int key;
        struct cur c;
        getmenuxy(mt, MM_COUNT, &c.cur_x, &c.cur_y);
        c.cur_x -= 2;
        c.pos = play_choise;
        show_cur(c);
        while ((key = getch()) != '\n') {
                switch (key) {
                case KEY_UP:
                        if (c.pos > 1)
                                move_cur(&c, -1);
                        break;
                case KEY_DOWN:
                        if (c.pos < 3)
                                move_cur(&c, 1);
                        break;
                }
        }
        return c.pos;
}

int mainmenu()
{
        int res;
        initcurses();
        timeout(-1);
        for (;;) {
                draw_mm(pn, mt, MM_COUNT);
                res = handle_mm();
                if (res == play_choise || res == exit_choise)
                        break;
        }
        endwin();
        return res;
}
