#include <stdio.h>
#include <curses.h>
#include <unistd.h>

#include "mainmenu.h"

enum {
        settings_count = 2,
        mainmenu_count = 3
};

double speed = 0.5;
int strength = 25;

static const char  program_name[]                = "Test";
static const char  mainmenu_text[][mm_bufsize]   = { "Play game", "Settings",
                                                     "Exit" };
static const char  settings_text[][mm_bufsize]   = { "Speed", "Strength" };
static const char  settings_range[][mm_bufsize]  = { "f", "0.25", "0", "1",
                                                     "i", "1", "0", "50" };
static const void *settings_ptr[]                = { &speed, &strength };
static const int   mainmenu_colors[mm_colors_count] = {
                                        COLOR_WHITE, COLOR_BLACK, A_STANDOUT,
                                        COLOR_WHITE, COLOR_BLACK, 0,
                                        COLOR_RED, COLOR_BLACK, A_BOLD,
                                        COLOR_BLACK, COLOR_RED, 0 };

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
        printf("Speed: %f\nStrength: %d\n", speed, strength);
        printf("...\n");
        sleep(2);
        initmainmenu(program_name, mainmenu_text, settings_text, settings_range,
                     settings_ptr, mainmenu_count, settings_count,
                     mainmenu_colors);
        initscr();
        for (;;) {
                res = mainmenu();
                initcurses();
                if (res == exit_choise)
                        break;
        }
        endwin();
        printf("Speed: %f\nStrength: %d\n", speed, strength);
}
