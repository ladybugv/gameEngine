#ifndef __STATUS_H__
#define __STATUS_H__
#include <string>
#include <array>
#include "statusview.h"

class Status
{
    std::array<std::string, 3> status_array;
    StatusView sv;

public:
    Status();
    void change_line(int i, std::string s);
    std::string get_line(int i);
    void display();
};

#endif
