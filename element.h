#ifndef __ELEMENT_H__
#define __ELEMENT_H__
#include <array>
#include <vector>
#include "elementview.h"
#include <functional>
#include <memory>

struct Physics
{
    // [0] is x, [1] is y, [2] is z
    std::array<double, 3> pos;
    std::array<double, 3> vel;
    std::array<double, 3> acc;
};

enum Shape
{
    Single,
    Rectangle,
    BitMap
};

enum Direction
{
    All,
    None,
    Left,
    Right,
    Top,
    Bottom
};

struct CharMap
{
    double x;
    double y;
    char c;
};

struct Form
{
    Shape shape;
    std::vector<CharMap> chars;
    int l;
    int w;
};

struct Collider
{
    Element *e;
    std::function<void(Element *)> f;
    bool collided;
    Direction direction;
};

class Element
{

    WINDOW *win;

protected:
    Physics p;
    bool isStatic; // does it move
    bool isCyclic; // does it change forms
    int cycle;     // current index of v the cycle is on
    int ticker;

public:
    std::vector<Form> forms;
    ElementView view;
    std::vector<Collider> colliders;
    Direction checkCollision(Collider &c);
    Direction current_collision;
    std::unique_ptr<Element> spawn;

    Element(WINDOW *win, double x, double y, double z, double x_v = 0, double y_v = 0, double z_v = 0, double x_a = 0, double y_a = 0, double z_a = 0);
    void display();

    int get_pos(int axis);
    int get_dimensions(int axis);
    const std::vector<Form> &get_forms();
    int get_cycle();

    void set_char(char c);
    void set_rectangle(char c, int x, int y);
    void set_bitmap(std::vector<CharMap> c);
    std::array<double, 3> &get_physics(int axis);

    virtual void tick();

    void add_collider(Element *e, std::function<void(Element *)> f, Direction d);
    void add_char(char c);
    void add_rectangle(char c, int x, int y);
    void add_bitmap(std::vector<CharMap> c);

    void remove_colliders();
    Element *spawner(double x, double y, double z, double x_v = 0, double y_v = 0, double z_v = 0, double x_a = 0, double y_a = 0, double z_a = 0);
    void kys();
    virtual ~Element();
};

#endif
