#ifndef MAINMENU_H
#define MAINMENU_H

enum {
        mm_bufsize  = 256,
        exit_choise = 255
};

void initmainmenu(const char  *program_name,
                  const char (*mainmenu_text)[mm_bufsize],
                  const char (*settings_text)[mm_bufsize],
                  const char (*settings_range)[mm_bufsize],
                  const void  *settings_pointer[],
                  const int    mainmenu_count,
                  const int    settings_count);
int  mainmenu();
#endif
