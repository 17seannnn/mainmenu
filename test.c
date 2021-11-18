#include <stdio.h>
#include <unistd.h>

#include "mainmenu.h"

static int speed, strength;

enum {
        settings_count = 2,
};

const char  mainmenu_text[][mm_bufsize]   = { "Play game", "Settings", "Exit" };
const char  settings_text[][mm_bufsize]   = { "Speed", "Strength" };
const char  settings_default[][mm_bufsize] = { "0.5", "25" };
const char  settings_range[][mm_bufsize]  = { "f", "0.25", "0", "1",
                                             "i", "1", "0", "50" };
const void *settings_ptr[]                = { &speed, &strength };

int main()
{
        int res;
        printf("Speed: %d\nStrength: %d\n", speed, strength);
        printf("...\n");
        sleep(2);
        initmainmenu(mainmenu_text, settings_text, settings_default, 
                     settings_range, settings_ptr, settings_count);
        for (;;) {
                res = mainmenu();
                if (res == exit_choise)
                        break;
        }
        printf("Speed: %d\nStrength: %d\n", speed, strength);
}
