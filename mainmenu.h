#ifndef MAINMENU_H
#define MAINMENU_H

enum {
        mm_bufsize  = 256,
        play_choise = 1,
        settings_choise,
        exit_choise
};

void initmainmenu(const char  *program_name,
                  const char (*mainmenu_text)[mm_bufsize],
                  const char (*settings_text)[mm_bufsize],
                  const char (*settings_range)[mm_bufsize],
                  const void  *settings_pointer[],
                  const int    settings_count);
int  mainmenu();
#endif
