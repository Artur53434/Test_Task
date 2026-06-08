#pragma once
#include <string>
#include "date.h"

struct Task
{
    int id = 0;
    std::string title = "";
    std::string text = "";
    Date date = Date();
    bool is_complite = false;

    std::string to_string();
    static Task from_string(const std::string& line);

    Task();
    Task(std::string title, std::string text, Date date);
};