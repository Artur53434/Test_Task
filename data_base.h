#pragma once
#include "task.h"
#include <vector>

struct Data_base
{
    Data_base();
    Data_base(std::string file_name);

    void add(Task task);

    void remove(int id);
    std::vector<Task> get_all();

    void done(int id);

    void undone(int id);
    void clear_completed();

private:
    int last_id = 0;
    const short LENGTH_LINE = 100;
    std::string file_name = "data_base.txt";

    void init_db();
    void update_header_in_file();

    void change_status(int id, bool status);
};

class file_open_error : public std::exception
{
public:

    file_open_error() {}
    file_open_error(const std::string& message) : message{ message } {}

    const char* what() const noexcept override
    {
        return message.c_str();
    }
private:
    std::string message = "Ошибка открытия файла";
};