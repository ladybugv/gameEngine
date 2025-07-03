#include "model.h"
#include "element.h"

std::vector<Element *> platforms;

bool can_jump();

int main(int argc, char *argv[])
{

    Model m;
    m.get_status().change_line(0, "Welcome to Geometry Dash!");
    m.get_status().change_line(1, "Press the UP key to jump.");
    m.get_status().change_line(2, " ");

    // player
    auto p = m.add_player(10, 15, 0, 0, 0, 0, 0, 0.1, 0);
    p->set_rectangle('#', 2, 2);

    // ground
    Element *ground = m.make_element(1, 20, 0);
    ground->set_rectangle('=', 80, 1);
    platforms.push_back(ground);

    ground->add_collider(p, [ground](Element *el)
                         { el->get_physics(1)[1] = 0; }, Direction::All);

    // background stars
    for (auto j : std::vector<std::pair<int, int>>{{10, 10}, {50, 15}, {30, 5}, {46, 9}, {52, 3}, {27, 13}, {4, 2}, {2, 18}, {72, 15}, {68, 8}})
    {
        Element *star = m.make_element(j.first, j.second, -5);
        star->set_char('+');
        star->add_char('*');
    }

    // spikes

    for (int i = 1; i < 5; i++)
    {
        Element *spike = m.make_element(20 + 15 * i, 18, 0, -0.5);
        spike->set_bitmap(std::vector<CharMap>{
            {0, 1, '/'},
            {2, 1, '\\'},
            {1, 0, '^'},
        });
        spike->add_collider(p, [&m](Element *e)
                            { e->set_rectangle('X', 2, 2);
                            m.play = false;
                            m.get_status().change_line(2, "You died! Press any key to leave."); }, Direction::All);
        // should be m.play = false instead of kys
    }

    // platforms
    for (int i = 0; i < 5; i++)
    {
        Element *base = m.make_element(100 + 10 * i, 17 - 3 * i, 0, -0.5);
        base->set_rectangle('|', 6, 3 + 3 * i);
        base->add_collider(p, [base, &m](Element *el)
                           {
            el->get_physics(1)[1] = 0;
            m.get_status().change_line(2, "You died! Press any key to leave."); 
            m.play=false; }, Direction::Left);
        base->add_collider(p, [base, &m](Element *el)
                           { el->get_physics(1)[1] = 0; }, Direction::All);
        platforms.push_back(base);
    }

    // platforms
    for (int i = 0; i < 2; i++)
    {
        Element *base = m.make_element(200 + 45 * i, 18, 0, -0.5);
        base->set_rectangle('|', 40, 2);
        base->add_collider(p, [base, &m](Element *el)
                           {
            el->get_physics(1)[1] = 0;
            m.get_status().change_line(2, "You died! Press any key to leave."); 
            m.play=false; }, Direction::Bottom);
        base->add_collider(p, [base, &m](Element *el)
                           {
            el->get_physics(1)[1] = 0;
            m.get_status().change_line(2, "You died! Press any key to leave."); 
            m.play=false; }, Direction::Left);
        base->add_collider(p, [base, &m](Element *el)
                           { el->get_physics(1)[1] = 0; }, Direction::All);
        platforms.push_back(base);

        Element *spike = m.make_element(215 + 45 * i, 16, 0, -0.5);
        spike->set_bitmap(std::vector<CharMap>{
            {0, 1, '/'},
            {2, 1, '\\'},
            {1, 0, '^'},
        });
        spike->add_collider(p, [&m](Element *e)
                            { e->set_rectangle('X', 2, 2);
                            m.play = false;
                            m.get_status().change_line(2, "You died! Press any key to leave."); }, Direction::All);
    }

    // platforms
    for (int i = 0; i < 4; i++)
    {
        Element *base = m.make_element(288 + 10 * i, 15 - i * 3, 0, -0.5);
        base->set_rectangle('|', 7, 1);
        base->add_collider(p, [base, &m](Element *el)
                           {
            el->get_physics(1)[1] = 0;
            m.get_status().change_line(2, "You died! Press any key to leave."); 
            m.play=false; }, Direction::Left);
        base->add_collider(p, [base, &m](Element *el)
                           {
            el->get_physics(1)[1] = 0;
            m.get_status().change_line(2, "You died! Press any key to leave."); 
            m.play=false; }, Direction::Bottom);
        base->add_collider(p, [base, &m](Element *el)
                           { el->get_physics(1)[1] = 0; }, Direction::All);
        platforms.push_back(base);
    }

    // spikes
    for (int i = 0; i < 12; i++)
    {
        Element *spike = m.make_element(290 + 5 * i, 18, 0, -0.5);
        spike->set_bitmap(std::vector<CharMap>{
            {0, 1, '/'},
            {2, 1, '\\'},
            {1, 0, '^'},
        });
        spike->add_collider(p, [&m](Element *e)
                            { e->set_rectangle('X', 2, 2);
                            m.play = false;
                            m.get_status().change_line(2, "You died! Press any key to leave."); }, Direction::All);
    }

    // boost
    Element *boost = m.make_element(336, 10, 0, -0.5);
    boost->set_bitmap(std::vector<CharMap>{
        {2, 0, '|'},
        {0, 1, '>'},
        {2, 1, 'O'},
        {4, 1, '<'},
        {2, 2, '|'},
    });
    boost->add_bitmap(std::vector<CharMap>{
        {1, 0, '^'},
        {0, 1, '-'},
        {2, 1, '-'},
        {4, 1, '-'},
        {1, 2, 'v'},
    });
    boost->add_collider(p, [boost, &m](Element *el) {}, Direction::All);
    platforms.push_back(boost);

    Element *finish = m.make_element(360, 1, 0, -0.5);
    finish->set_rectangle('|', 1, 20);
    finish->add_collider(p, [&m](Element *el)
                         { m.play = false; 
                            m.get_status().change_line(2, "You win! Press any key to leave."); }, Direction::All);

    // jump
    p->add_interaction(KEY_UP, [ground](Element *e)
                       { for (auto p : platforms)
    {
        if (p->current_collision != Direction::None)
        {
            e->get_physics(1)[1] = -1;
            return;
        }
    }
    return; });
    // p->add_key_motion(KEY_DOWN, 1, 0, 1);
    // p->add_key_motion(KEY_LEFT, 0, 0, -1);
    // p->add_key_motion(KEY_RIGHT, 0, 0, 1);

    m.go();
}

void make_spike()
{
}
