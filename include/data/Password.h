#pragma once

#include <string>
#include <optional>
#include <cstdint>
#include <utility>

struct Password
{
    std::string name;
    std::string value;
    uint16_t category_id;
    std::optional<std::string> service;
    std::optional<std::string> login;
};

using PasswordId = int;
using PasswordRecord = std::pair<PasswordId, Password>;
