#include <stdio.h>
#include <curses.h>
#include <locale.h>
#include <libintl.h>

#define gettext_noop(STR) STR
#define TEXTDOMAIN "mmtest"
#define LOCALEDIR "/usr/share/locale"

#include "mainmenu.h"

static double speed;
static int strength, distance, luck, mana, xp, gold;

/* program name */
static const char  pn[]             = gettext_noop("Test");
/* file name */
static const char  fn[]             = ".testcfg";
/* mainmenu text */
static const char  mt[][mm_bufsize] = {
                                 gettext_noop("Play game"),
                                 gettext_noop("Button"),
                                 gettext_noop("Button"),
                                 gettext_noop("Button"),
                                 gettext_noop("Button"),
                                 gettext_noop("Button"),
                                 gettext_noop("Settings"),
                                 gettext_noop("Exit")
};
/* settings text */
static const char  st[][mm_bufsize] = {
                                 gettext_noop("Speed"),
                                 gettext_noop("Strength"),
                                 gettext_noop("Distance"),
                                 gettext_noop("Luck"),
                                 gettext_noop("Mana"),
                                 gettext_noop("Xp"),
                                 gettext_noop("Gold")
};
/* settings range */
static const char  sr[][mm_bufsize] = {
                                 "f", "0.25", "0.25", "0", "1",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50",
                                 "i", "25", "1", "0", "50"
};
/* settings pointers */
static      void *sp[]              = {
                                 &speed, &strength, &distance, &luck,
                                 &mana, &xp, &gold
};
/* mainmenu/settings count */
static const int   mc = 8, sc = 7;
/* mainmenu colors */
static const int   mm_colors[mm_colors_count] = {
                                /* Program name */
                                 COLOR_WHITE, COLOR_BLACK, A_STANDOUT,
                                /* Default text */
                                 COLOR_WHITE, COLOR_BLACK, 0,
                                /* Cursor */
                                 COLOR_RED, COLOR_BLACK, A_BOLD,
                                /* Highlighted param */
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
        initmm(pn, fn, mt, st, sr, sp, mc, sc, mm_colors);
        for (;;) {
                res = mainmenu();
                initcurses();
                if (res == exit_choise)
                        break;
        }
        endwin();
        return 0;
}
