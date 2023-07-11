#pragma once

#include <string>
#include <optional>

class PasswordGenerator
{
public:
    bool use_uppercase = false;
    bool use_lowercase = false;
    bool use_digits = false;
    bool use_special_characters = false;

    std::optional<std::string> generate(size_t length) const;

private:
    char generate_char() const;
    bool matches_parameters(char ch) const;
};
