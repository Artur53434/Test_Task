#include "task.h"
#include <sstream>
#include <string>
#include <iostream>

std::string Task::to_string()
{
    std::string res;

    res += std::to_string(this->id) + "\x1F";
    res += this->title + "\x1F";
    res += this->text + "\x1F";
    res += this->date.to_string() + "\x1F";
    if (this->is_complite) res += "true";
    else res += "false";

    return res;
}

Task::Task() {}

Task::Task(std::string title, std::string text, Date date)
{
    this->id = 0;
    this->title = title;
    this->text = text;
    this->date = date;
    this->is_complite = false;
}

Task Task::from_string(const std::string& line)
{
    Task task;

    std::string clean_line = line;
    int endpos = clean_line.find_last_not_of(" \n\r");
    if (endpos != std::string::npos) {
        clean_line = clean_line.substr(0, endpos + 1);
    }
    else {
        return task;
    }

    if (clean_line.find("DB_HEADER") != std::string::npos) {
        return task;
    }

    std::stringstream ss(clean_line);
    std::string token;

    try {
        if (std::getline(ss, token, '\x1F')) {
            if (!token.empty()) task.id = std::stoi(token);
        }
        if (std::getline(ss, token, '\x1F')) task.title = token;
        if (std::getline(ss, token, '\x1F')) task.text = token;

        if (std::getline(ss, token, '\x1F')) {
            std::stringstream date_ss(token);
            std::string d, m, y;
            std::getline(date_ss, d, '.');
            std::getline(date_ss, m, '.');
            std::getline(date_ss, y, '.');

            if (!d.empty()) task.date.day = std::stoi(d);
            if (!m.empty()) task.date.month = std::stoi(m);
            if (!y.empty()) task.date.year = std::stoi(y);
        }

        if (std::getline(ss, token, '\x1F')) {
            task.is_complite = (token == "true");
        }
    }
    catch (const std::exception& e) {}

    return task;
}