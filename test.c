#include <stdio.h>
#include <unistd.h>

#include "mainmenu.h"

enum {
        settings_count = 2,
        mainmenu_count = 3
};

static double speed = 0.5;
static int strength = 25;

static const char  program_name[]                = "Test";
static const char  mainmenu_text[][mm_bufsize]   = { "Play game", "Settings",
                                                     "Exit" };
static const char  settings_text[][mm_bufsize]   = { "Speed", "Strength" };
static const char  settings_range[][mm_bufsize]  = { "f", "0.25", "0", "1",
                                                     "i", "1", "0", "50" };
static const void *settings_ptr[]                = { &speed, &strength };

int main()
{
        int res;
        printf("Speed: %f\nStrength: %d\n", speed, strength);
        printf("...\n");
        sleep(2);
        initmainmenu(program_name, mainmenu_text, settings_text, settings_range,
                     settings_ptr, mainmenu_count, settings_count);
        for (;;) {
                res = mainmenu();
                if (res == exit_choise)
                        break;
        }
        printf("Speed: %f\nStrength: %d\n", speed, strength);
}
