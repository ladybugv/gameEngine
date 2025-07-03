#ifndef __MAINSCREENVIEW_H__
#define __MAINSCREENVIEW_H__

#include <ncurses.h>
#include "view.h"

class MainScreenView : public View
{
    WINDOW *win;
    const int WIDTH = 80;
    const int HEIGHT = 22;

public:
    void display_view();
    void update_view();
    void end_view();
    void clear_view();
    WINDOW *get_window();
};

#endif
