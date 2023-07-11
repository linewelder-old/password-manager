#pragma once

#include <string>
#include <vector>
#include <functional>
#include <ctime>

template <typename T>
using Predicate = std::function<bool(const T&)>;

template <typename T>
using Comparator = std::function<int(const T&, const T&)>;

bool is_special_character(char x);
bool contains_ignore_case(const std::string& string, const std::string& substring);
int compare_ignore_case(const std::string& x, const std::string& y);

int euclidean_modulo(int x, int y);

template <typename T, typename U>
std::vector<U> map(
    const std::vector<T>& values,
    std::function<U(const T&)> mapper)
{
    std::vector<U> result;
    for (const auto& value : values)
    {
        result.push_back(mapper(value));
    }

    return result;
}

template <typename T>
static Predicate<T> all_of(const std::vector<Predicate<T>>& predicates)
{
    return [=](const T& x)
    {
        for (const auto& predicate : predicates)
        {
            if (!predicate(x))
            {
                return false;
            }
        }

        return true;
    };
}

template <typename T>
static Comparator<T> comparator_sequence(const std::vector<Comparator<T>>& comparators)
{
    return [=](const T& x, const T& y)
    {
        for (const auto& comparator : comparators)
        {
            const auto result = comparator(x, y);
            if (result != 0)
            {
                return result;
            }
        }

        return 0;
    };
}
