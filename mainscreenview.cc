#include <ncurses.h>
#include "mainscreenview.h"

void MainScreenView::display_view()
{
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    curs_set(0);
    win = newwin(22, 80, 1, 10);
    refresh();
    box(win, 0, 0);
    wrefresh(win);
}

void MainScreenView::update_view() {}

void MainScreenView::end_view()
{
    endwin();
}

void MainScreenView::clear_view()
{
    for (int i = 1; i < HEIGHT - 1; i++)
    {
        for (int j = 1; j < WIDTH - 1; j++)
        {
            mvwaddch(win, i, j, ' ');
        }
    }
    wrefresh(win);
};

WINDOW *MainScreenView::get_window()
{
    return win;
};
