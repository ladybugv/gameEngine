#ifndef __STATUSVIEW_H__
#define __STATUSVIEW_H__

#include <ncurses.h>
#include "view.h"

class Status;

class StatusView : public View
{
    WINDOW *status;
    Status &s;
    const int WIDTH = 80;
    const int HEIGHT = 5;
    void clear_line(int line);

public:
    StatusView(Status &s);
    void display_view();
    void update_view();
};

#endif
