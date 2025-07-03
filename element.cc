#include "element.h"
#include <iostream>
#include <cmath>
#include "model.h"

Element::~Element() {
};

// create Element object
Element::Element(WINDOW *win, double x, double y, double z, double x_v, double y_v, double z_v, double x_a, double y_a, double z_a) : win(win),
                                                                                                                                      view{std::move(ElementView{win, this})},
                                                                                                                                      p(std::move(Physics{{x, y, z}, {x_v, y_v, z_v}, {x_a, y_a, z_a}})),
                                                                                                                                      isCyclic(false),
                                                                                                                                      isStatic(true),
                                                                                                                                      cycle(0),
                                                                                                                                      forms({}),
                                                                                                                                      ticker(0),
                                                                                                                                      current_collision(Direction::None)
{
    forms = {(Form{Shape::Single, std::vector<CharMap>{CharMap{0, 0, 'X'}}, 1, 1})};
}

// display element
void Element::display()
{
    view.display_view();
}

// return position
int Element::get_pos(int axis)
{
    return p.pos[axis];
};

// return list of all forms
const std::vector<Form> &Element::get_forms()
{
    return forms;
};

// get current form cycle index
int Element::get_cycle()
{
    return cycle;
};

// set display to a character
void Element::set_char(char c)
{
    cycle = 0;
    forms.clear();
    add_char(c);
    isCyclic = false;
};

// set display to a rectangle
void Element::set_rectangle(char c, int x, int y)
{
    cycle = 0;
    forms.clear();
    add_rectangle(c, x, y);
    isCyclic = false;
}

// set display to a bitmap
void Element::set_bitmap(std::vector<CharMap> c)
{
    cycle = 0;
    forms.clear();
    add_bitmap(c);
    isCyclic = false;
};

// add character form to a cycle
void Element::add_char(char c)
{
    isCyclic = true;
    forms.push_back(std::move(Form{Shape::Single, std::vector<CharMap>{CharMap{0, 0, c}}, 1, 1}));
};

// add rectangle form to a cycle
void Element::add_rectangle(char c, int x, int y)
{
    isCyclic = true;
    forms.push_back(std::move(Form{Shape::Rectangle, std::vector<CharMap>{}, x, y}));
    int len = forms.size() - 1;
    for (double i = 0; i < x; ++i)
    {
        for (double j = 0; j < y; ++j)
        {
            forms[len].chars.push_back(CharMap{i, j, c});
        }
    }
};

// add bitmap form to a cycle
void Element::add_bitmap(std::vector<CharMap> c)
{
    isCyclic = true;
    forms.push_back(std::move(Form{Shape::BitMap, std::vector<CharMap>{}, 0, 0}));
    int len = forms.size() - 1;
    int l_max = 0;
    int w_max = 0;
    for (auto x : c)
    {
        if (x.x > l_max)
        {
            l_max = x.x;
        }
        if (x.y > w_max)
        {
            w_max = x.y;
        }
        forms[len].chars.push_back(x);
        forms[len].l = l_max;
        forms[len].w = w_max;
    }
};

// increment cycle, check collisions, and increment position each tick
void Element::tick()
{
    ticker = (ticker + 1) % 3;
    if (isCyclic && ticker == 0)
    {
        if (cycle == forms.size() - 1)
        {
            cycle = 0;
        }
        else
        {
            cycle++;
        }
    }

    for (Collider x : colliders)
    {
        Direction dir = checkCollision(x);
        if (dir == x.direction || (dir != Direction::None && x.direction == Direction::All))
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

// return element's current dimensions
int Element::get_dimensions(int axis)
{
    if (axis == 0)
    {
        return forms[cycle].l;
    }
    else
    {
        return forms[cycle].w;
    }
};

// add collider
void Element::add_collider(Element *e, std::function<void(Element *)> f, Direction d)
{
    colliders.push_back(Collider{e, std::move(f), false, d});
};

// check for a current collision
Direction Element::checkCollision(Collider &c)
{
    Element *e = c.e;
    int x = e->get_pos(0);
    int y = e->get_pos(1);
    if (p.pos[2] == e->get_pos(2)) // z overlaps
    {
        if ((p.pos[0] + get_dimensions(0) >= e->get_pos(0)) && p.pos[0] <= e->get_pos(0) + e->get_dimensions(0)) // x overlaps
        {
            if ((p.pos[1] + get_dimensions(1) >= e->get_pos(1)) && p.pos[1] <= e->get_pos(1) + e->get_dimensions(1)) // y overlaps
            {
                c.collided = true;
                if ((std::floor(p.pos[0]) + get_dimensions(0) == std::floor(e->get_pos(0))))
                {
                    current_collision = Direction::Right;
                }
                else if (std::floor(p.pos[0]) == std::floor(e->get_pos(0)) + e->get_dimensions(0))
                {
                    current_collision = Direction::Left;
                }
                else if ((std::floor(p.pos[1]) + get_dimensions(1) == std::floor(e->get_pos(1))))
                {
                    current_collision = Direction::Bottom;
                }
                else if (std::floor(p.pos[1]) == std::floor(e->get_pos(1)) + e->get_dimensions(1))
                {
                    current_collision = Direction::Top;
                }
                else
                {
                    current_collision = Direction::All;
                }
                return current_collision;
            }
        }
    }
    current_collision = Direction::None;
    return current_collision;
};

// return physics
std::array<double, 3> &Element::get_physics(int axis)
{
    if (axis == 0)
    {
        return p.pos;
    }
    else if (axis == 1)
    {
        return p.vel;
    }
    else
    {
        return p.acc;
    }
};

// spawn new object
Element *Element::spawner(double x, double y, double z, double x_v, double y_v, double z_v, double x_a, double y_a, double z_a)
{
    spawn = std::move(std::make_unique<Element>(win, x, y, z, x_v, y_v, z_v, x_a, y_a, z_a));
    return spawn.get();
};

// empty set of colliders
void Element::remove_colliders()
{
    colliders = {};
};

// kys
void Element::kys()
{
    set_bitmap({});
    remove_colliders();
    p = {{0, 0, -100}, {0, 0, 0}, {0, 0, 0}};
};
