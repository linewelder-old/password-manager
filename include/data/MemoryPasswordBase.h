#pragma once

#include <vector>
#include <map>
#include "data/Password.h"
#include "data/PasswordBase.h"
#include "util/utils.h"

class MemoryPasswordBase : public PasswordBase
{
    std::map<PasswordId, Password> passwords;
    std::map<CategoryId, std::string> categories;

    PasswordId next_password_id = 0;
    CategoryId next_category_id = 0;

public:
    MemoryPasswordBase();

    PasswordId add_password(Password password) override;
    CategoryId add_category(std::string name) override;
    void insert_category(CategoryId id, std::string name) override;

    void remove_password(PasswordId id) override;
    void remove_category(CategoryId id) override;

    Password get_password(PasswordId id) const override;
    std::string get_category(CategoryId id) const override;

    std::vector<PasswordRecord> fetch_passwords(
        const std::vector<SearchParameter>& search_parameters,
        const std::vector<PasswordProperty>& sorting_parameters) const override;

    std::vector<CategoryRecord> fetch_all_categories() const override;
    CategoryId get_category_id(std::string name) const override;

private:
    std::optional<CategoryId> find_category(const std::string& name) const;

    std::vector<PasswordRecord> get_all_passwords() const;
    std::vector<PasswordRecord> find_where(const Predicate<Password>& predicate) const;

    Predicate<Password> get_predicate(const SearchParameter& parameter) const;
    Predicate<Password> get_searching_predicate(
        const std::vector<SearchParameter>& parameters) const;

    Comparator<Password> get_comparator(PasswordProperty property) const;
    Comparator<Password> get_sorting_comparator(
        const std::vector<PasswordProperty>& parameters) const;

    friend class EncryptedWriter;
};
