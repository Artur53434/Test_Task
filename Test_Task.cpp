#include <iostream>
#include <stdio.h>
#include <vector>
#include <conio.h>
#include <fstream>
#include <cstdio>
#include <string.h>
#include <Windows.h>
#include <ctime>
#include "data_base.h"
#include "task.h"
#include "menu.h"

using namespace std;

int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    Data_base* data_base = new Data_base();
    Menu menu;

    vector<string> list_menu;
    list_menu.push_back("Добавить заметку");
    list_menu.push_back("Удалить заметку");
    list_menu.push_back("Вывести все заметки");
    list_menu.push_back("Удалить все выполненые заметки");
    list_menu.push_back("Изменить файл для сохрания заметок");
    list_menu.push_back("Вывести статистику по выполнению задач");
    list_menu.push_back("Пометить задачу выполненной/невыполненной");
    list_menu.push_back("Выход");

    int point = 0;
    bool is_exit = false;

    while (is_exit == false) {
        system("cls");
        menu.Print_Menu(list_menu, point);

        menu.Set_Color_Consol(COLOR_GRAY, COLOR_BLACK);
        menu.Reset_Color();

        int key = _getch();

        if (key == 0xE0 || key == 0x00) {
            key = _getch();
        }

        switch (key) {
        case UP:
            point = (point - 1 + SIZE_MENU) % SIZE_MENU;
            break;
        case DOWN:
            point = (point + 1) % SIZE_MENU;
            break;
        case ENTER:
            system("cls");
            switch (point) {
            case 0: {
                printf("--- ДОБАВЛЕНИЕ ЗАМЕТКИ ---\n");
                menu.Output_Colot_Text(COLOR_YELLOW, "(Введите 0 для отмены)\n\n");
                try {
                    std::string title = menu.Valid_String_Input("Введите заголовок (макс. 20 символов): ", 20, true);
                    std::string text = menu.Valid_String_Input("Введите содержание (макс. 50 символов): ", 50, true);

                    time_t now = time(0);
                    tm ltm = {};
                    localtime_s(&ltm, &now);

                    short cur_day = ltm.tm_mday;
                    short cur_month = 1 + ltm.tm_mon;
                    short cur_year = 1900 + ltm.tm_year;

                    std::string day_prompt = "Введите день (1-31) [Enter: " + std::to_string(cur_day) + "]: ";
                    std::string month_prompt = "Введите месяц (1-12) [Enter: " + std::to_string(cur_month) + "]: ";
                    std::string year_prompt = "Введите год (2000-2100) [Enter: " + std::to_string(cur_year) + "]: ";

                    short day = menu.Valid_Int_Input(day_prompt, 1, 31, cur_day, true);
                    short month = menu.Valid_Int_Input(month_prompt, 1, 12, cur_month, true);
                    short year = menu.Valid_Int_Input(year_prompt, 2000, 2100, cur_year, true);

                    Task new_task(title, text, Date(day, month, year));
                    try {
                        data_base->add(new_task);
                        menu.Output_Colot_Text(COLOR_GREEN, "\nЗаметка успешно добавлена!\n");
                    }
                    catch (const file_open_error& e) {
                        menu.Output_Colot_Text(COLOR_RED, e.what());
                        printf("\n");
                    }
                }
                catch (const user_сancelled&) {
                    menu.Output_Colot_Text(COLOR_YELLOW, "\nДобавление заметки отменено.\n");
                }
                break;
            }
            case 1: {
                printf("--- УДАЛЕНИЕ ЗАМЕТКИ ---\n");
                try {
                    std::vector<Task> tasks = data_base->get_all();
                    if (tasks.empty()) {
                        menu.Output_Colot_Text(COLOR_YELLOW, "Список заметок пуст.\n");
                    }
                    else {
                        for (size_t i = 0; i < tasks.size(); i++) {
                            menu.Print_Task(tasks[i], i + 1);
                        }
                        int index = menu.Valid_Int_Input("Введите номер заметки для удаления (0 - отмена): ", 0, tasks.size());
                        if (index != 0) {
                            int target_id = tasks[index - 1].id;
                            data_base->remove(target_id);
                            menu.Output_Colot_Text(COLOR_GREEN, "Заметка удалена.\n");
                        }
                    }
                }
                catch (const file_open_error& e) {
                    menu.Output_Colot_Text(COLOR_RED, e.what());
                    printf("\n");
                }
                break;
            }
            case 2: {
                printf("--- СПИСОК ВСЕХ ЗАМЕТОК ---\n");
                try {
                    std::vector<Task> tasks = data_base->get_all();
                    if (tasks.empty()) {
                        menu.Output_Colot_Text(COLOR_YELLOW, "Список заметок пуст.\n");
                    }
                    else {
                        for (int i = 0; i < tasks.size(); i++) {
                            menu.Print_Task(tasks[i], i + 1);
                        }
                    }
                }
                catch (const file_open_error& e) {
                    menu.Output_Colot_Text(COLOR_RED, e.what());
                    printf("\n");
                }
                break;
            }
            case 3: {
                printf("--- ОЧИСТКА ВЫПОЛНЕННЫХ ЗАДАЧ ---\n");
                int confirm = menu.Valid_Int_Input("Вы уверены, что хотите удалить ВСЕ выполненные задачи? (1 - Да, 0 - Нет): ", 0, 1);
                if (confirm == 1) {
                    try {
                        data_base->clear_completed();
                        menu.Output_Colot_Text(COLOR_GREEN, "Все выполненные заметки удалены.\n");
                    }
                    catch (const file_open_error& e) {
                        menu.Output_Colot_Text(COLOR_RED, e.what());
                        printf("\n");
                    }
                }
                else {
                    menu.Output_Colot_Text(COLOR_YELLOW, "Операция отменена.\n");
                }
                break;
            }
            case 4: {
                printf("--- СМЕНА ФАЙЛА БАЗЫ ДАННЫХ ---\n");
                menu.Output_Colot_Text(COLOR_YELLOW, "(Введите 0 для отмены)\n\n");
                try {
                    std::string new_file = menu.Valid_String_Input("Введите новое имя файла (например, new_db.txt): ", 50, true);
                    delete data_base;
                    data_base = new Data_base(new_file);
                    menu.Output_Colot_Text(COLOR_GREEN, "Файл для сохранения изменен на " + new_file + "\n");
                }
                catch (const user_сancelled&) {
                    menu.Output_Colot_Text(COLOR_YELLOW, "\nСмена файла отменена.\n");
                }
                break;
            }
            case 5: {
                printf("--- СТАТИСТИКА ---\n");
                try {
                    std::vector<Task> tasks = data_base->get_all();
                    int total = tasks.size();
                    int completed = 0;
                    for (int i = 0; i < tasks.size(); i++) {
                        if (tasks[i].is_complite) completed++;
                    }
                    int uncompleted = total - completed;

                    printf("Всего задач: %d\n", total);
                    menu.Output_Colot_Text(COLOR_GREEN, "Выполнено: " + std::to_string(completed) + "\n");
                    menu.Output_Colot_Text(COLOR_YELLOW, "Не выполнено: " + std::to_string(uncompleted) + "\n");

                    if (total > 0) {
                        double percent = (static_cast<double>(completed) / total) * 100.0;
                        printf("Процент выполнения: %.2f%%\n", percent);
                    }
                }
                catch (const file_open_error& e) {
                    menu.Output_Colot_Text(COLOR_RED, e.what());
                    printf("\n");
                }
                break;
            }
            case 6: {
                printf("--- ИЗМЕНЕНИЕ СТАТУСА ЗАДАЧИ ---\n");
                try {
                    std::vector<Task> tasks = data_base->get_all();
                    if (tasks.empty()) {
                        menu.Output_Colot_Text(COLOR_YELLOW, "Список заметок пуст.\n");
                    }
                    else {
                        for (int i = 0; i < tasks.size(); i++) {
                            menu.Print_Task(tasks[i], i + 1);
                        }
                        int index = menu.Valid_Int_Input("Введите номер заметки (0 - отмена): ", 0, tasks.size());
                        if (index != 0) {
                            int target_id = tasks[index - 1].id;
                            bool current_status = tasks[index - 1].is_complite;

                            if (current_status) {
                                data_base->undone(target_id);
                                menu.Output_Colot_Text(COLOR_GREEN, "Задача отмечена как НЕ ВЫПОЛНЕННАЯ.\n");
                            }
                            else {
                                data_base->done(target_id);
                                menu.Output_Colot_Text(COLOR_GREEN, "Задача отмечена как ВЫПОЛНЕННАЯ.\n");
                            }
                        }
                    }
                }
                catch (const file_open_error& e) {
                    menu.Output_Colot_Text(COLOR_RED, e.what());
                    printf("\n");
                }
                break;
            }
            case 7:
                is_exit = true;
                break;
            }

            if (point != 7) {
                menu.Output_Colot_Text(COLOR_GRAY, "\nДля продолжения нажмите любую кнопку...\n");
                _getch();
            }
            break;
        case ESC:
            is_exit = true;
            break;
        default:
            break;
        }
    }

    delete data_base;
    return 0;
}