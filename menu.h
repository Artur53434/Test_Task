#pragma once
#include <string>
#include <vector>
#include "task.h"

#define SIZE_MENU 8
#define CODE_ERROR_INPUT 400
#define CODE_GOOD_INPUT 1
#define STR_INCORECT_INPUT "Не коректный ввод. Пожалуйста повторите попитку."
#define STR_VALUE_BEYOND \
    "Введённые данные находятся за пределами допустимых значений. Пожалуйста введите значение согласно " \
    "инструкциям."
#define STR_LINE \
    "--------------------------------------------------------------------------------"

enum keys_menu
{
    UP = 72,
    DOWN = 80,
    ENTER = 13,
    ESC = 27
};
enum color
{
    COLOR_GRAY = 8,
    COLOR_WHITE = 15,
    COLOR_BLACK = 0,
    COLOR_RED = 12,
    COLOR_GREEN = 10,
    COLOR_YELLOW = 14
};

struct Limit
{
    int max;
    int min;
};

class user_сancelled : public std::exception {};

struct Menu
{
    void Print_Menu(std::vector<std::string> list_menu, int point);
    void Valid_Input(double* input, struct Limit bounds, char* text);
    void Clear_Input_Buffer();
    void Set_Color_Consol(int text_color, int bg_color);
    void Reset_Color();
    void Output_Colot_Text(int text_color, std::string text);

    std::string Valid_String_Input(const std::string& prompt, int max_len, bool allow_cancel = false);
    int Valid_Int_Input(const std::string& prompt, int min_val, int max_val, int default_val = -1, bool allow_cancel = false);
    void Print_Task(Task& task, int index);
};