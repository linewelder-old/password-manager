#pragma once

#include <string>
#include <vector>
#include <functional>
#include <cmath>
#include "ui/printing.h"

template <typename T>
using FieldWidthGetter = std::function<unsigned int(const T&)>;

template <typename T>
using FieldValueGetter = std::function<std::string(const T&)>;

template <typename T>
struct Column
{
    std::string title;
    unsigned int max_width;
    FieldValueGetter<T> field_value_getter;
};

template <typename T>
class Table
{
    std::vector<Column<T>> columns;
    unsigned int column_gap;
    unsigned int right_padding;

public:
    Table(const std::vector<Column<T>>& columns,
        unsigned int column_gap, unsigned int right_padding)
        : columns(columns), column_gap(column_gap),
          right_padding(right_padding) {}

    void print(const std::vector<T>& entries) const
    {
        std::vector<unsigned int> column_widths;
        unsigned int full_width = 0;
        for (const auto& column : columns)
        {
            const unsigned int column_width =
                get_column_width(column, entries);

            column_widths.push_back(column_width);
            full_width += column_width;
        }

        const unsigned int max_index_length =
            get_length_in_characters(entries.size());
        const unsigned int index_column_width = max_index_length + 2;

        full_width += index_column_width
                    + (column_gap * columns.size() - 1) + right_padding;

        print_n_times(' ', index_column_width);
        print_row_without_index(
            column_widths,
            [](const Column<T>& column) { return column.title; }
        );

        print_n_times('=', full_width);
        printf("\n");

        for (unsigned int i = 0; i < entries.size(); i++)
        {
            const auto& entry = entries.at(i);

            print_list_index(i + 1, max_index_length);
            print_row_without_index(
                column_widths,
                [&](const Column<T>& column) { return column.field_value_getter(entry); }
            );
        }
    }

private:
    void print_row_without_index(
        const std::vector<unsigned int> column_widths,
        std::function<std::string(const Column<T>&)> field_value_getter) const
    {
        for (unsigned int i = 0; i < columns.size(); i++)
        {
            print_fitted(column_widths.at(i), field_value_getter(columns.at(i)));
            print_n_times(' ', column_gap);
        }
        printf("\n");
    }

    static unsigned int get_column_width(
        const Column<T>& column, const std::vector<T>& entries)
    {
        unsigned int max_field_width = 0;
        for (const auto& entry : entries)
        {
            const unsigned int field_width =
                column.field_value_getter(entry).size();
            if (field_width > max_field_width)
            {
                max_field_width = field_width;
            }
        }

        return std::clamp(
            max_field_width,
            (unsigned int)column.title.size(), column.max_width);
    }
};
