#include "util/utils.h"

#include <algorithm>
#include <cstdlib>
#include <ctime>

static std::string to_lowercase(const std::string& string)
{
    std::string result(string.size(), ' ');
    std::transform(string.begin(), string.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

bool is_special_character(char x)
{
    return x >= '!' && x <= '/'
        || x >= ':' && x <= '@'
        || x >= '[' && x <= '`'
        || x >= '{' && x <= '~';
}

bool contains_ignore_case(const std::string& string, const std::string& substring)
{
    return to_lowercase(string).find(to_lowercase(substring)) != std::string::npos;
}

int compare_ignore_case(const std::string& x, const std::string& y)
{
    return to_lowercase(x).compare(to_lowercase(y));
}

int euclidean_modulo(int x, int y)
{
    int value = x % y;
    return value >= 0 ? value : value + std::abs(y);
}
