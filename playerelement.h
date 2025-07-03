#ifndef __PLAYER_ELEMENT_H__
#define __PLAYER_ELEMENT_H__

#include "element.h"

struct Motion
{
    int key;
    int axis;
    int physics;
    double distance;
    std::function<bool(Element *)> condition;
};

struct Interaction
{
    int key;
    std::function<void(Element *)> f;
};

class PlayerElement;

class PlayerElement : public Element
{
    std::vector<Motion> keybinds;
    std::vector<Interaction> keyinteracts;

    void check_keybinds(int key);

public:
    PlayerElement(WINDOW *win, double x, double y, double z, double x_v = 0, double y_v = 0, double z_v = 0, double x_a = 0, double y_a = 0, double z_a = 0);
    void add_key_motion(int key, int axis, int physics, double difference, std::function<bool(Element *)> condition = [](Element *e)
                                                                           { return true; });
    void add_interaction(int key, std::function<void(Element *)> f);
    void tick() override;
};

#endif
