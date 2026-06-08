#pragma once
#include <string>

struct Date
{
	short day;
	short month;
	short year;

	std::string to_string();

	Date();
	Date(short day, short month, short year);
};
