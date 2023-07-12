#pragma once

#include <ctime>
#include <string>
#include <functional>
#include "ui/DefaultExceptionPrinter.h"

void print_time(time_t timestamp);
unsigned int get_length_in_characters(unsigned int  value);
void print_list_index(unsigned int index, unsigned int list_length);
void print_n_times(char ch, unsigned int times);
void print_fitted(unsigned int column_width, const std::string& value);

void print_exception(const std::string title, const PrintableException& ex);

template <typename T>
void print_list(const std::vector<T>& values,
    std::function<std::string(T)> value_formatter)
{
    unsigned int max_index_length =
        get_length_in_characters(values.size());

    for (unsigned int i = 0; i < values.size(); i++)
    {
        print_list_index(i + 1, max_index_length);
        std::cout << value_formatter(values.at(i)) << '\n';
    }
}
