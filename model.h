#ifndef __MODEL_H__
#define __MODEL_H__

#include <memory>
#include "element.h"
#include "playerelement.h"
#include "mainscreenview.h"
#include "status.h"

class Model
{
    std::vector<std::unique_ptr<Element>> elements;
    MainScreenView m;
    Status s;
    WINDOW *w;
    std::unique_ptr<PlayerElement> p;
    void tick();

public:
    Model();
    void go();
    Status &get_status();
    void delete_element(Element &e);
    bool play;
    int score;

    template <typename... Args>
    Element *make_element(double x, double y, double z, Args &&...args)
    {
        for (int i = 0; i < elements.size(); ++i)
        {
            if (z <= elements[i]->get_pos(2))
            {
                elements.insert(elements.begin() + i, std::move(std::make_unique<Element>(w, x, y, z, args...)));
                return elements[i].get();
            }
        }

        elements.push_back(std::move(std::make_unique<Element>(w, x, y, z, args...)));
        return elements[elements.size() - 1].get();
    };

    template <typename... Args>
    PlayerElement *add_player(Args... args)
    {
        p = std::make_unique<PlayerElement>(w, args...);
        return p.get();
    };
};

#endif
