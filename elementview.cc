#include "elementview.h"
#include "element.h"
#include <iostream>

ElementView::ElementView(WINDOW *screen, Element *s) : screen(screen), s(s) {};

// display element
void ElementView::display_view()
{

    auto forms = s->forms;

    int c = s->get_cycle();
    for (auto x : forms[c].chars)
    {
        place_char(s->get_pos(0) + x.x, s->get_pos(1) + x.y, x.c);
    }
    wrefresh(screen);
};

void ElementView::update_view() {};

// draw character on screen if within bounds
void ElementView::place_char(int x, int y, char c)
{
    if (x > 0 && y > 0 && x < 79 && y < 21)
    {
        mvwaddch(screen, y, x, c);
    }
};
