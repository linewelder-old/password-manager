#pragma once

#include <vector>
#include <map>
#include <filesystem>
#include "data/Password.h"
#include "data/PasswordBase.h"
#include "data/MemoryPasswordBase.h"
#include "util/utils.h"

class FilePasswordBase : public PasswordBase
{
    std::filesystem::path file;
    std::string password;
    PasswordBase& underlying_base;
    time_t last_opening_time;

public:
    static FilePasswordBase create_new(
        std::filesystem::path file, std::string password, PasswordBase& underlying_base);
    static FilePasswordBase load_from_file(
        std::filesystem::path file, std::string password, PasswordBase& underlying_base);

    time_t get_last_opening_time() const;

    PasswordId add_password(Password password);
    CategoryId add_category(std::string name);
    void insert_category(CategoryId id, std::string name);

    void remove_password(PasswordId id);
    void remove_category(CategoryId id);

    Password get_password(PasswordId id) const;
    std::string get_category(CategoryId id) const;

    std::vector<PasswordRecord> fetch_passwords(
        const std::vector<SearchParameter>& search_parameters,
        const std::vector<PasswordProperty>& sorting_parameters) const;

    std::vector<CategoryRecord> fetch_all_categories() const;
    CategoryId get_category_id(std::string name) const;

private:
    FilePasswordBase(std::filesystem::path file,
        std::string password, PasswordBase& underlying_base);

    void save() const;
};
