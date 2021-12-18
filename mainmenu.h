#ifndef MAINMENU_H
#define MAINMENU_H

enum {
        mm_bufsize      = 256,
        mm_colors_count = 12,
        exit_choise     = 255
};

void initmainmenu(const char  *program_name,
                  const char (*mainmenu_text)[mm_bufsize],
                  const char (*settings_text)[mm_bufsize],
                  const char (*settings_range)[mm_bufsize],
                  const void  *settings_pointer[],
                  const int    mainmenu_count,
                  const int    settings_count,
                  const int    mainmenu_colors[mm_colors_count]);
int  mainmenu();

#endif
