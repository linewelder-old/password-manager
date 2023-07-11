#include "data/PasswordGenerator.h"

#include <sstream>
#include <random>
#include "util/utils.h"

std::optional<std::string> PasswordGenerator::generate(size_t length) const
{
    if (!use_uppercase && !use_lowercase
     && !use_digits && !use_special_characters)
    {
        return std::nullopt;
    }

    std::ostringstream ss;
    for (size_t i = 0; i < length; i++)
    {
        ss << generate_char();
    }

    return ss.str();
}

char PasswordGenerator::generate_char() const
{
    char ch;
    do
    {
        ch = rand() % 128;
    }
    while (!matches_parameters(ch));

    return ch;
}

bool PasswordGenerator::matches_parameters(char x) const
{
    return use_uppercase && x >= 'A' && x <= 'Z'
        || use_lowercase && x >= 'a' && x <= 'z'
        || use_digits    && x >= '0' && x <= '9'
        || use_special_characters && is_special_character(x);
}
