#include <stdio.h>
#include <unistd.h>
#include <curses.h>

#include "mainmenu.h"

double speed;
int strength;

const char  pn[]             = "Test";
const char  fn[]             = ".testcfg";
const char  mt[][mm_bufsize] = { "Play game", "Settings", "Exit" };
const char  st[][mm_bufsize] = { "Speed", "Strength" };
const char  sr[][mm_bufsize] = { "f", "0.25", "0.25", "0", "1",
                                 "i", "25", "1", "0", "50" };
      void *sp[]             = { &speed, &strength };
const int   mc = 3, sc = 2;
const int   mm_colors[mm_colors_count] = {
                                 COLOR_WHITE, COLOR_BLACK, A_STANDOUT,
                                 COLOR_WHITE, COLOR_BLACK, 0,
                                 COLOR_RED, COLOR_BLACK, A_BOLD,
                                 COLOR_RED, COLOR_BLACK, 0
};

static void initcurses()
{
        noecho();
        cbreak();
        if (has_colors())
                start_color();
        keypad(stdscr, 1);
        curs_set(1);
}

int main()
{
        int res;
        initscr();
        for (;;) {
                res = mainmenu();
                initcurses();
                if (res == exit_choise)
                        break;
        }
        endwin();
}
