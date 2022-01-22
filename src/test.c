#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <locale.h>
#include <libintl.h>

#define _(STR) gettext(STR)
#define _N(STR) STR
#define gettext_noop(STR) STR
#define TEXTDOMAIN "mmtest"
#define LOCALEDIR "/usr/share/locale"

#include "mainmenu.h"

double speed;
int strength, distance, luck, mana, xp, gold;

/* program name */
const char  pn[]             = gettext_noop("Test");
/* file name */
const char  fn[]             = ".testcfg";
/* mainmenu text */
const char  mt[][mm_bufsize] = { gettext_noop("Play game"),
                                 gettext_noop("Settings"),
                                 gettext_noop("Exit")
};
/* settings text */
const char  st[][mm_bufsize] = { gettext_noop("Speed"),
                                 gettext_noop("Strength"),
                                 gettext_noop("Distance"),
                                 gettext_noop("Luck"),
                                 gettext_noop("Mana"),
                                 gettext_noop("Xp"),
                                 gettext_noop("Gold")
};
/* settings range */
const char  sr[][mm_bufsize] = { "f", "0.25", "0.25", "0", "1",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50"
};
/* settings pointers */
      void *sp[]             = { &speed, &strength, &distance, &luck,
                                 &mana, &xp, &gold
};
/* mainmenu/settings count */
const int   mc = 3, sc = 7;
/* mainmenu colors */
const int   mm_colors[mm_colors_count] = {
                                 COLOR_WHITE, COLOR_BLACK, A_STANDOUT,
                                 COLOR_WHITE, COLOR_BLACK, 0,
                                 COLOR_RED, COLOR_BLACK, A_BOLD,
                                 COLOR_RED, COLOR_BLACK, 0
};

static void initgettext()
{
        setlocale(LC_CTYPE, "");
        setlocale(LC_MESSAGES, "");
        bindtextdomain(TEXTDOMAIN, LOCALEDIR);
        textdomain(TEXTDOMAIN);
}

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
        initgettext();
        initscr();
        for (;;) {
                res = mainmenu();
                initcurses();
                if (res == exit_choise)
                        break;
        }
        endwin();
}
