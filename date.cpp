#include "date.h"

std::string Date::to_string() {
    return std::to_string(this->day) + "." 
        + std::to_string(this->month) + "." 
        + std::to_string(this->year);
}

Date::Date() {
    this->day = 0;
    this->month = 0;
    this->year = 0;
}

Date::Date(short day, short month, short year) {
    this->day = day;
    this->month = month;
    this->year = year;
}