#include "ui/printing.h"

#include <cstdio>
#include <cmath>
#include <ctime>
#include <string>
#include <iomanip>
#include <iostream>
#include "ui/DefaultExceptionPrinter.h"

void print_time(std::time_t timestamp)
{
    std::tm* time = std::localtime(&timestamp);
    std::cout << std::put_time(time, "%d.%m.%Y %H:%M:%S");
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

void print_exception(const std::string title, const PrintableException& ex)
{
    DefaultExceptionPrinter printer(std::cout);

    std::cout << title << ": ";
    ex.print(printer);
    std::cout << '\n';
}
