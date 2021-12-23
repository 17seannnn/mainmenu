#ifndef CONFIG_H
#define CONFIG_H

#include "mainmenu.h"

double speed = 0.5;
int strength = 25;

const char  pn[]             = "Test";
const char  mt[][mm_bufsize] = { "Play game", "Settings", "Exit" };
const char  st[][mm_bufsize] = { "Speed", "Strength" };
const char  sr[][mm_bufsize] = { "f", "0.25", "0", "1", "i", "1", "0", "50" };
const void *sp[]             = { &speed, &strength };
const int   mc = 3, sc = 2;
const int   mm_colors[mm_colors_count] = {
                                 COLOR_WHITE, COLOR_BLACK, A_STANDOUT,
                                 COLOR_WHITE, COLOR_BLACK, 0,
                                 COLOR_RED, COLOR_BLACK, A_BOLD,
                                 COLOR_RED, COLOR_BLACK, 0
};

#endif
