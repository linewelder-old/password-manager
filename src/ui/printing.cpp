#include "ui/printing.h"

#include <cstdio>
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
#include "ui/DefaultExceptionPrinter.h"

void print_time(std::time_t timestamp)
{
    std::tm time;
    time = *std::localtime(&timestamp);

    printf(
        "%02d.%02d.%d %d:%02d:%02d",
        time.tm_mday, time.tm_mon + 1, time.tm_year + 1900,
        time.tm_hour, time.tm_min, time.tm_sec
    );
}

unsigned int get_length_in_characters(unsigned int value)
{
    return (unsigned int)std::log10(value) + 1;
}

void print_list_index(unsigned int index, unsigned int list_length)
{
    printf(
        "%*d. ",
        list_length, index
    );
}

void print_n_times(char ch, unsigned int times)
{
    for (unsigned int i = 0; i < times; i++)
    {
        printf("%c", ch);
    }
}

void print_fitted(unsigned int column_width, const std::string& value)
{
    if (value.size() > column_width)
    {
        printf("%.*s", column_width - 3, value.c_str());
        print_n_times('.', 3);
    }
    else
    {
        printf("%-*s", column_width, value.c_str());
    }
}

void print_exception(const std::string tytul, const PrintableException& ex)
{
    DefaultExceptionPrinter printer(std::cout);

    std::cout << tytul << ": ";
    ex.print(printer);
    std::cout << '\n';
}
