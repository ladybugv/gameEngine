#ifndef __VIEW_H__
#define __VIEW_H__

class View
{
public:
    virtual void display_view() = 0;
    virtual void update_view() = 0;
    virtual ~View() {};
};

#endif
