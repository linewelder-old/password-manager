#pragma once

#include <string>
#include <vector>
#include "Password.h"
#include "Category.h"

enum class PasswordProperty
{
    NAME,
    CATEGORY,
    SERVICE,
    LOGIN
};

struct SearchParameter
{
    PasswordProperty property;
    std::string query;
};

class PasswordBase
{
public:
    /**
    * \exception InvalidDataException
    */
    virtual PasswordId add_password(Password password) = 0;

    /**
    * \exception InvalidDataException
    */
    virtual CategoryId add_category(std::string name) = 0;

    /**
    * \exception InvalidDataException Kategoria o takim id lub takiej nazwie juz istnieje
    */
    virtual void insert_category(CategoryId id, std::string name) = 0;

    /**
    * \exception InvalidDataException Nie ma hasla o takim id
    */
    virtual void remove_password(PasswordId id) = 0;

    /**
    * \exception InvalidDataException Nie ma kategorii o takim id
    */
    virtual void remove_category(CategoryId id) = 0;

    /**
    * \exception InvalidDataException Nie ma hasla o takim id
    */
    virtual Password get_password(PasswordId id) const = 0;

    /**
    * \exception InvalidDataException Nie ma kategorii o takim id
    */
    virtual std::string get_category(CategoryId id) const = 0;
    
    /**
    * \param search_parameters Gdy pusty, zwraca wszystkie hasla
    * \param sorting_parameters Gdy pusty, zwraca w dowolnej kolejnosci
    * \exeption InvalidFindSortParameterException
    * \exeption UnknownCategoryNameException
    */
    virtual std::vector<PasswordRecord> fetch_passwords(
        const std::vector<SearchParameter>& search_parameters,
        const std::vector<PasswordProperty>& sorting_parameters) const = 0;

    virtual std::vector<CategoryRecord> fetch_all_categories() const = 0;
    virtual CategoryId get_category_id(std::string name) const = 0;
};
