#include "playerelement.h"
#include <fstream>

PlayerElement::PlayerElement(WINDOW *win, double x, double y, double z, double x_v, double y_v, double z_v, double x_a, double y_a, double z_a) : Element{win,
                                                                                                                                                          x,
                                                                                                                                                          y,
                                                                                                                                                          z,
                                                                                                                                                          x_v,
                                                                                                                                                          y_v,
                                                                                                                                                          z_v,
                                                                                                                                                          x_a,
                                                                                                                                                          y_a,
                                                                                                                                                          z_a} {};

void PlayerElement::add_key_motion(int key, int axis, int physics, double difference, std::function<bool(Element *)> condition)
{
    keybinds.push_back(Motion{key, axis, physics, difference, condition});
};

void PlayerElement::tick()
{
    nodelay(stdscr, TRUE);

    int c = getch();
    if (c != ERR)
    {
        check_keybinds(c);
    }
    for (Collider x : colliders)
    {
        if (checkCollision(x))
        {
            x.f(x.e);
        }
    }

    for (int i = 0; i < 3; i++)
    {
        p.pos[i] += p.vel[i];
        p.vel[i] += p.acc[i];
    }
};

void PlayerElement::add_interaction(int key, std::function<void(Element *)> f)
{
    keyinteracts.push_back(Interaction{key, f});
};

void PlayerElement::check_keybinds(int key)
{
    for (auto x : keybinds)
    {
        if (x.key == key)
        {
            if (x.condition(this))
            {
                if (x.physics == 0) // distance
                {
                    std::ofstream of("aaa.txt");
                    double np = p.pos[x.axis] + x.distance;

                    if ((x.axis == 0 && np > 0 && np < 79 - forms.at(cycle).l) || (x.axis == 1 && np > 0 && np < 21))
                    {
                        of << np;
                        p.pos[x.axis] = np;
                    }
                }
                else if (x.physics == 1)
                {
                    p.vel[x.axis] += x.distance;
                }
                else if (x.physics == 2)
                {
                    p.acc[x.axis] += x.distance;
                }
                return;
            }
        }
    };
    for (auto x : keyinteracts)
    {
        if (x.key == key)
        {
            x.f(this);
        }
    }
};
