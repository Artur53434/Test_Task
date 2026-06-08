#include "data_base.h"
#include <fstream>
#include <filesystem> 
#include <sstream>
#include <iostream>
#include <vector>

Data_base::Data_base()
{
    init_db();
}

Data_base::Data_base(std::string file_name)
{
    this->file_name = file_name;
    init_db();
}

void Data_base::add(Task task)
{
    this->last_id++;
    task.id = this->last_id;
    update_header_in_file();

    std::ofstream file(this->file_name, std::ios::app | std::ios::binary);
    if (!file.is_open()) throw file_open_error();

    std::string line = task.to_string();

    while (line.size() < this->LENGTH_LINE - 1) line.append(" ");
    line.append("\n");

    file << line;

    file.close();
}

void Data_base::remove(int id)
{
    std::fstream file(this->file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) throw file_open_error();

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();

    if (file_size < this->LENGTH_LINE * 2) return;
    if (file_size == 0 || file_size % this->LENGTH_LINE != 0) return;

    int total_lines = file_size / this->LENGTH_LINE;
    int target_index = -1;

    std::vector<char> buffer(this->LENGTH_LINE);

    for (int i = 1; i < total_lines; i++)
    {
        file.seekg(i * this->LENGTH_LINE);
        file.read(buffer.data(), this->LENGTH_LINE);

        std::string line_str(buffer.data(), this->LENGTH_LINE);

        size_t delim_pos = line_str.find('\x1F');
        if (delim_pos != std::string::npos)
        {
            try {
                std::string id_str = line_str.substr(0, delim_pos);
                int current_id = std::stoi(id_str);
                if (current_id == id)
                {
                    target_index = i;
                    break;
                }
            }
            catch (...) {
                continue;
            }
        }
    }

    if (target_index == -1) return;

    if (target_index != total_lines - 1)
    {
        file.seekg((total_lines - 1) * this->LENGTH_LINE);
        file.read(buffer.data(), this->LENGTH_LINE);

        file.seekp(target_index * this->LENGTH_LINE);
        file.write(buffer.data(), this->LENGTH_LINE);
    }

    file.close();
    std::filesystem::resize_file(this->file_name, (int)file_size - this->LENGTH_LINE);
}

std::vector<Task> Data_base::get_all()
{
    std::vector<Task> tasks;

    std::ifstream file(this->file_name, std::ios::in | std::ios::binary);
    if (!file.is_open()) throw file_open_error();

    std::string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            Task t = Task::from_string(line);
            if (t.id != 0) {
                tasks.push_back(t);
            }
        }
    }

    file.close();
    return tasks;
}

void Data_base::done(int id)
{
    this->change_status(id, true);
}

void Data_base::undone(int id)
{
    this->change_status(id, false);
}

void Data_base::clear_completed()
{
    std::fstream file(this->file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) throw file_open_error();

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();

    if (file_size < this->LENGTH_LINE * 2) return;

    int total_lines = file_size / this->LENGTH_LINE;

    int write_index = 1;

    std::vector<char> buffer(this->LENGTH_LINE);

    for (int read_index = 1; read_index < total_lines; ++read_index)
    {
        file.seekg(read_index * this->LENGTH_LINE);
        file.read(buffer.data(), this->LENGTH_LINE);

        std::string line_str(buffer.data(), this->LENGTH_LINE);
        Task task = Task::from_string(line_str);

        if (!task.is_complite)
        {
            if (write_index != read_index)
            {
                file.seekp(write_index * this->LENGTH_LINE);
                file.write(buffer.data(), this->LENGTH_LINE);
            }
            write_index++;
        }
    }

    file.close();

    std::filesystem::resize_file(this->file_name, write_index * this->LENGTH_LINE);
}

void Data_base::init_db()
{
    std::ifstream check_file(this->file_name, std::ios::binary);

    if (!check_file.is_open() || std::filesystem::file_size(this->file_name) < this->LENGTH_LINE) {
        check_file.close();
        std::ofstream new_file(this->file_name, std::ios::binary);
        this->last_id = 0;

        std::string header = "DB_HEADER\x1F" + std::to_string(this->last_id) + "\x1F";
        while (header.size() < this->LENGTH_LINE - 1) header.append(" ");
        header.append("\n");

        new_file << header;
        new_file.close();
    }
    else {
        std::vector<char> buffer(this->LENGTH_LINE);
        check_file.read(buffer.data(), this->LENGTH_LINE);
        std::string header_str(buffer.data(), this->LENGTH_LINE);
        check_file.close();

        std::stringstream ss(header_str);
        std::string token;
        std::getline(ss, token, '\x1F');

        try {
            if (std::getline(ss, token, '\x1F')) {
                if (!token.empty()) {
                    this->last_id = std::stoi(token);
                }
                else {
                    this->last_id = 0;
                }
            }
        }
        catch (...) {
            this->last_id = 0;
        }
    }
}

void Data_base::update_header_in_file()
{
    std::fstream file(this->file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) throw file_open_error();

    std::string header = "DB_HEADER\x1F" + std::to_string(this->last_id) + "\x1F";
    while (header.size() < this->LENGTH_LINE - 1) header.append(" ");
    header.append("\n");

    file.seekp(0);
    file.write(header.c_str(), this->LENGTH_LINE);
    file.close();
}

void Data_base::change_status(int id, bool status)
{
    std::fstream file(this->file_name, std::ios::in | std::ios::out | std::ios::binary);
    if (!file.is_open()) throw file_open_error();

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();

    if (file_size < this->LENGTH_LINE * 2) return;

    int total_lines = file_size / this->LENGTH_LINE;
    std::vector<char> buffer(this->LENGTH_LINE);

    for (int i = 1; i < total_lines; ++i)
    {
        file.seekg(i * this->LENGTH_LINE);
        file.read(buffer.data(), this->LENGTH_LINE);

        std::string line_str(buffer.data(), this->LENGTH_LINE);

        int delim_pos = line_str.find('\x1F');
        if (delim_pos != std::string::npos)
        {
            try {
                int current_id = std::stoi(line_str.substr(0, delim_pos));

                if (current_id == id)
                {
                    Task task = Task::from_string(line_str);

                    task.is_complite = status;

                    std::string new_line = task.to_string();
                    while (new_line.size() < this->LENGTH_LINE - 1) new_line.append(" ");
                    new_line.append("\n");

                    file.seekp(i * this->LENGTH_LINE);
                    file.write(new_line.c_str(), this->LENGTH_LINE);

                    break;
                }
            }
            catch (...) {
                continue;
            }
        }
    }
    file.close();
}