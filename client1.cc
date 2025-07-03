#include "model.h"
#include "element.h"

std::vector<Element *> aliens;

void new_alien(int *i, Model &m, Element *p, double x, double y, double z, double shift, double fall);

int main(int argc, char *argv[])
{
    int score = 0;
    const int SCORE = 17;

    Model m;
    m.get_status().change_line(0, "Welcome to Space Invaders!");
    m.get_status().change_line(1, "Press space to shoot, and L/R to control your player.");
    m.get_status().change_line(2, "Don't let the aliens get to you!");

    // player
    auto p = m.add_player(40, 19, -1);
    p->set_bitmap(std::vector<CharMap>{
        {0, 0, '/'},
        {1, 0, '\\'},
        {0, 1, '='},
        {1, 1, '='},
    });

    // aliens

    for (int i = 1; i < 4; i++)
    {
        new_alien(&score, m, p, 20 * i, 3, -1, -0.1, 0.03);
    }

    for (int i = 1; i < 4; i++)
    {
        new_alien(&score, m, p, 20 * i - 10, -5, -1, 0, 0.03);
    }

    for (int i = 1; i < 4; i++)
    {
        new_alien(&score, m, p, 20 * i, -7, -1, 0.1, 0.05);
    }

    for (int i = 1; i < 5; i++)
    {
        new_alien(&score, m, p, 20 * i - 15, -15, -1, 0, 0.05);
    }

    Element *dummy = m.make_element(20, -15, -1, 0, 0.05);
    dummy->set_char(' ');

    for (int i = 1; i < 6; i++)
    {
        new_alien(&score, m, p, 20 * i - 13, -17, -1, 0, 0.05);
    }

    // lower border
    Element *bottom = m.make_element(1, 20, -1);
    bottom->set_rectangle(' ', 78, 1);
    for (auto a : aliens)
    {
        bottom->add_collider(a, [&m, s = &score](Element *el)
                             {
        m.play = false;
        
        m.get_status().change_line(0, "You lost! Press any key to leave.");
        m.get_status().change_line(1, "SCORE:");
        m.get_status().change_line(2, std::to_string(*s)); }, Direction::Top);
    }
    bottom->add_collider(dummy, [&m, s = &score](Element *el)
                         {
        m.play = false;
        
        m.get_status().change_line(0, "You win! Press any key to leave.");
        m.get_status().change_line(1, "SCORE:");
        m.get_status().change_line(2, std::to_string(*s)); }, Direction::Top);

    // sorry this is actually the right border
    Element *left = m.make_element(78, 1, -1);
    left->set_rectangle(' ', 1, 22);
    for (auto a : aliens)
    {
        left->add_collider(a, [](Element *el)
                           { el->get_physics(1)[0] *= -1; }, Direction::All);
    }

    // and this is the left i just don't know my lefts and rights
    Element *right = m.make_element(0, 1, -1);
    right->set_rectangle(' ', 1, 22);
    for (auto a : aliens)
    {
        right->add_collider(a, [](Element *el)
                            { el->get_physics(1)[0] *= -1; }, Direction::All);
    }

    // shots
    p->add_interaction((' '), [s = &score, mm = &m](Element *e)
                       { auto x = e->spawner(e->get_pos(0), e->get_pos(1), -1, 0, -0.5); 
                            x->set_char('|'); 
                            for (auto y : aliens) {
                                x->add_collider(y, [x, s](Element *e){
                                    if (x->get_pos(2) > -50) {
                                        (*s)++;
                                    }
                                    if (x) x->kys();
                                    if (e) e->kys();
                                },Direction::All);
                            } });

    p->add_key_motion(KEY_LEFT, 0, 0, -1);
    p->add_key_motion(KEY_RIGHT, 0, 0, 1);

    m.go();
}

// make a new alien and add it to the list of aliens
void new_alien(int *s, Model &m, Element *p, double x, double y, double z, double shift, double fall)
{
    Element *alien = m.make_element(x, y, z, shift, fall);
    alien->set_bitmap(std::vector<CharMap>{
        {1, 0, '^'},
        {2, 0, '^'},
        {0, 1, '/'},
        {3, 1, '\\'},
        {1, 1, 'o'},
        {2, 1, 'o'}});
    for (int i = 0; i < 5; i++)
    {
        alien->add_bitmap(std::vector<CharMap>{
            {1, 0, '^'},
            {2, 0, '^'},
            {0, 1, '/'},
            {3, 1, '\\'},
            {1, 1, 'o'},
            {2, 1, 'o'}});
    }
    alien->add_bitmap(std::vector<CharMap>{
        {1, 0, '^'},
        {2, 0, '^'},
        {0, 1, '/'},
        {3, 1, '\\'},
        {1, 1, '-'},
        {2, 1, '-'}});

    alien->add_collider(p, [&m, s](Element *e)
                        { e->set_rectangle('X', 2, 2);
                            m.play = false;
                            m.get_status().change_line(0, "You died! Press any key to leave.");
                            m.get_status().change_line(1, "SCORE:");
                            m.get_status().change_line(2, std::to_string(*s)); }, Direction::All);
    aliens.push_back(alien);
}
