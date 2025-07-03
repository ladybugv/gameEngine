#ifndef __ELEMENTVIEW_H__
#define __ELEMENTVIEW_H__

#include <ncurses.h>
#include "view.h"

class Element;

class ElementView : public View
{
    WINDOW *screen;
    Element *s;

public:
    ElementView(WINDOW *screen, Element *s);
    void display_view();
    void update_view();
    void place_char(int x, int y, char c);
};

#endif
