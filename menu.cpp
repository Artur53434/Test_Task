#include "menu.h"

#include <vector>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>

void Menu::Valid_Input(double* input, Limit bounds, char* text) {
    int code_input;
    do {
        code_input = CODE_GOOD_INPUT;
        std::cout << text;
        std::cin >> *input;
        int next_char = getchar();
        Set_Color_Consol(COLOR_RED, COLOR_BLACK);

        if (next_char != '\n') {
            if (next_char != EOF) Clear_Input_Buffer();
            printf("%s\n", STR_INCORECT_INPUT);
            code_input = CODE_ERROR_INPUT;
        }
        else if (*input < bounds.min || *input > bounds.max) {
            printf("%s\n", STR_VALUE_BEYOND);
            code_input = CODE_ERROR_INPUT;
        }

        Reset_Color();
    } while (code_input != CODE_GOOD_INPUT);
}

void Menu::Clear_Input_Buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void Menu::Print_Menu(std::vector<std::string> list_menu, int point) {
    printf("Для навигации по меню используйте стрелочки вверх и вниз.\n");
    printf("Нажмите enter для выбора пункта меню.\n");
    printf("Меню:\n");
    printf("%s\n", STR_LINE);
    for (int i = 0; i < SIZE_MENU; i++) {
        if (point == i)
            printf("->  %d - %-38s", i + 1, list_menu[i].c_str());
        else
            printf("    %d - %-38s", i + 1, list_menu[i].c_str());
        printf("\n");
    }
    printf("%s\n", STR_LINE);
}

void Menu::Set_Color_Consol(int text_color, int bg_color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bg_color << 4) | text_color);
}

void Menu::Reset_Color() {
    Set_Color_Consol(COLOR_WHITE, COLOR_BLACK);
}

void Menu::Output_Colot_Text(int text_color, std::string text) {
    Set_Color_Consol(text_color, COLOR_BLACK);
    printf("%s", text.c_str());
    Reset_Color();
}

std::string Menu::Valid_String_Input(const std::string& prompt, int max_len, bool allow_cancel) {
    std::string input;
    while (true) {
        printf("%s", prompt.c_str());
        std::getline(std::cin, input);

        size_t start = input.find_first_not_of(" \t");
        if (start == std::string::npos) {
            input = "";
        }
        else {
            input = input.substr(start, input.find_last_not_of(" \t") - start + 1);
        }

        if (allow_cancel && input == "0") {
            throw user_сancelled();
        }

        if (input.empty()) {
            Set_Color_Consol(COLOR_RED, COLOR_BLACK);
            printf("Поле не может быть пустым. Повторите ввод.\n");
            Reset_Color();
        }
        else if (input.length() > max_len) {
            Set_Color_Consol(COLOR_RED, COLOR_BLACK);
            printf("Слишком длинный ввод. Максимум %d символов. Повторите ввод.\n", max_len);
            Reset_Color();
        }
        else {
            break;
        }
    }
    return input;
}

int Menu::Valid_Int_Input(const std::string& prompt, int min_val, int max_val, int default_val, bool allow_cancel) {
    int input;
    std::string line;
    while (true) {
        printf("%s", prompt.c_str());
        std::getline(std::cin, line);

        int start = line.find_first_not_of(" \t");
        if (start == std::string::npos) {
            line = "";
        }
        else {
            line = line.substr(start, line.find_last_not_of(" \t") - start + 1);
        }

        if (line.empty() && default_val != -1) {
            return default_val;
        }
        else if (line.empty()) {
            Set_Color_Consol(COLOR_RED, COLOR_BLACK);
            printf("%s\n", STR_INCORECT_INPUT);
            Reset_Color();
            continue;
        }

        if (allow_cancel && line == "0") {
            throw user_сancelled();
        }

        std::stringstream ss(line);

        if (!(ss >> input)) {
            Set_Color_Consol(COLOR_RED, COLOR_BLACK);
            printf("%s\n", STR_INCORECT_INPUT);
            Reset_Color();
            continue;
        }

        std::string leftover;
        if (ss >> leftover) {
            Set_Color_Consol(COLOR_RED, COLOR_BLACK);
            printf("%s\n", STR_INCORECT_INPUT);
            Reset_Color();
            continue;
        }

        if (input < min_val || input > max_val) {
            Set_Color_Consol(COLOR_RED, COLOR_BLACK);
            printf("%s От %d до %d\n", STR_VALUE_BEYOND, min_val, max_val);
            Reset_Color();
        }
        else {
            break;
        }
    }
    return input;
}

void Menu::Print_Task(Task& task, int index) {
    printf("%d. Заголовок: %s | Срок: %s | Статус: ",
        index, task.title.c_str(), task.date.to_string().c_str());

    if (task.is_complite) {
        Output_Colot_Text(COLOR_GREEN, "Выполнено\n");
    }
    else {
        Output_Colot_Text(COLOR_YELLOW, "Не выполнено\n");
    }

    printf("   Текст: %s\n", task.text.c_str());
    printf("%s\n", STR_LINE);
}