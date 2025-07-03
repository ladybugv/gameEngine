#include <string>
#include "status.h"

Status::Status() : sv(StatusView{*this}) {};

void Status::change_line(int i, std::string s)
{
    status_array[i] = s;
    sv.update_view();
}

void Status::display()
{
    sv.display_view();
}

std::string Status::get_line(int i)
{
    return status_array[i];
};
