#include "ui/utils.h"

#include <filesystem>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <optional>
#include <sstream>
#include <algorithm>
#include <functional>
#include "data/Password.h"
#include "data/Category.h"
#include "ui/Table.h"

namespace fs = std::filesystem;
using uint = unsigned int;

const int COLUMN_GAP = 2;
const int RIGHT_PADDING = 3;

static uint get_column_width(
    const std::vector<PasswordRecord>& passwords,
    uint min_width, uint max_width,
    std::function<uint(const Password&)> field_width_getter)
{
    uint max_field_width = 0;
    for (const auto& record : passwords)
    {
        const uint field_width = field_width_getter(record.second);
        if (field_width > max_field_width)
        {
            max_field_width = field_width;
        }
    }

    return std::clamp(max_field_width, min_width, max_width);
}

static std::string get_category_name(const std::vector<CategoryRecord>& categories, CategoryId id)
{
    const auto& found_record = std::find_if(
        categories.begin(), categories.end(),
        [=](const CategoryRecord& record) { return record.first == id; }
    );

    return found_record->second;
}

void print_password_table(const std::vector<CategoryRecord>& categories, const std::vector<PasswordRecord>& passwords)
{
    const Table<PasswordRecord> table(
        {
            { "Nazwa",     20,
                [] (const PasswordRecord& x) { return x.second.name; } },
            { "Kategoria", 15,
                [&](const PasswordRecord& x) { return get_category_name(categories, x.second.category_id); } },
            { "Strona",    15,
                [] (const PasswordRecord& x) { return x.second.service.value_or("-"); } },
            { "Login",     17,
                [] (const PasswordRecord& x) { return x.second.login.value_or("-"); }}
        },
        COLUMN_GAP, RIGHT_PADDING
    );

    table.print(passwords);
}

bool is_considered_path(const std::string& string)
{
    return std::find_if(
        string.begin(), string.end(),
        [](char x) { return !isalpha(x) && !isdigit(x) && x != '-' && x != '_'; }
    ) != string.end();
}

std::string get_stripped_filename(const fs::path& path)
{
    return path.filename().replace_extension().string();
}

std::vector<std::filesystem::path> get_files_with_extension(
    const fs::path& directory, const std::string& extension)
{
    std::vector<fs::path> base_files;
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == extension)
        {
            base_files.push_back(entry.path());
        }
    }

    return base_files;
}

std::optional<int> try_read_int(const std::string& str)
{
    std::stringstream ss(str);

    int value;
    ss >> value;
    if (ss.fail() || ss.get() != -1)
    {
        return std::nullopt;
    }

    return value;
}
