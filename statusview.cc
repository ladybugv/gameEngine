#include <ncurses.h>
#include "view.h"
#include "statusview.h"
#include "status.h"
#include <fstream>

StatusView::StatusView(Status &s) : status(nullptr), s(s)
{
    status = newwin(5, 80, 23, 10);
};

void StatusView::display_view()
{
    status = newwin(5, 80, 23, 10);

    for (int i = 0; i < 3; ++i)
    {
        mvwprintw(status, i, 1, "%s", s.get_line(i).c_str());
    }

    wrefresh(status);

    // getch();
    // endwin();
};

void StatusView::update_view()
{
    for (int i = 0; i < 3; ++i)
    {
        clear_line(i);
        mvwprintw(status, i, 1, "%s", s.get_line(i).c_str());
    }
    wrefresh(status);
};

void StatusView::clear_line(int line)
{
    for (int i = 1; i < WIDTH - 1; i++)
    {
        mvwaddch(status, line, i, ' ');
    }
};
