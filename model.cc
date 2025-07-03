#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <algorithm>

#include "mainscreenview.h"
#include "status.h"
#include "element.h"
#include "model.h"

Model::Model()
{
    m.display_view();
    s.display();
    w = m.get_window();
    play = true;
}

// run game
void Model::go()
{
    int counter = 0;
    int maxCalls = 10000;

    while (counter < maxCalls && play)
    {
        tick();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        counter++;
    }

    s.display();

    nodelay(stdscr, FALSE);
    getch();
    m.end_view();
};

void Model::tick()
{
    // escape game
    int ch = getch();
    if (ch == 27)
    {
        play = false;
        return;
    }
    else
    {
        ungetch(ch);
    }

    // refresh screen
    m.clear_view();

    // update elements and call display and take spawns
    for (auto &e : elements)
    {
        e->tick();
        e->display();

        if (e->spawn != nullptr)
        {
            bool added = false;
            for (int i = 0; i < elements.size(); ++i)
            {
                if (e->spawn->get_pos(2) <= elements[i]->get_pos(2))
                {
                    elements.insert(elements.begin() + i, std::move(e->spawn));
                    added = true;
                    break;
                }
            }
            if (!added)
            {
                elements.push_back(std::move(e->spawn));
            }
            e->spawn = nullptr;
        }
    }
    if (p)
    {
        p->tick();
        p->display();

        if (p->spawn != nullptr)
        {
            bool added = false;
            for (int i = 0; i < elements.size(); ++i)
            {
                if (p->spawn->get_pos(2) <= elements[i]->get_pos(2))
                {
                    elements.insert(elements.begin() + i, std::move(p->spawn));
                    added = true;
                    break;
                }
            }
            if (!added)
            {
                elements.push_back(std::move(p->spawn));
            }
            p->spawn = nullptr;
        }
    }
}

Status &Model::get_status()
{
    return s;
};

void Model::delete_element(Element &e)
{
    for (int i = 0; i < elements.size(); ++i)
    {
        if (elements[i].get() == &e)
        {
            elements.erase(elements.begin() + i);
            return;
        };
    }
};
