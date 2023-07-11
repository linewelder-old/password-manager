#include "data/MemoryPasswordBase.h"

#include <algorithm>
#include "data/exceptions/InvalidDataException.h"
#include "data/exceptions/InvalidFindSortParameterException.h"
#include "data/exceptions/UnknownCategoryNameException.h"
#include "data/Password.h"
#include "util/utils.h"

MemoryPasswordBase::MemoryPasswordBase()
    : passwords(), categories() {}

PasswordId MemoryPasswordBase::add_password(Password password)
{
    if (password.name.empty())
    {
        throw InvalidDataException(InvalidDataExceptionType::EMPTY_NAME);
    }

    if (password.value.empty())
    {
        throw InvalidDataException(InvalidDataExceptionType::EMPTY_PASSWORD);
    }

    if (categories.find(password.category_id) == categories.end())
    {
        throw InvalidDataException(InvalidDataExceptionType::UNKNOWN_CATEGORY_ID);
    }

    if (password.login.has_value() && password.login.value().empty())
    {
        password.login = std::nullopt;
    }

    if (password.service.has_value() && password.service.value().empty())
    {
        password.service = std::nullopt;
    }

    passwords.insert({ next_password_id++, password });
    return next_password_id;
}

CategoryId MemoryPasswordBase::add_category(std::string name)
{
    if (name.empty())
    {
        throw InvalidDataException(InvalidDataExceptionType::EMPTY_CATEGORY_NAME);
    }

    if (find_category(name).has_value())
    {
        throw InvalidDataException(InvalidDataExceptionType::CATEGORY_NAME_EXISTS);
    }

    insert_category(next_category_id++, name);
    return next_category_id;
}

void MemoryPasswordBase::insert_category(CategoryId id, std::string name)
{
    const bool category_id_exists = categories.find(id) != categories.end();
    if (category_id_exists)
    {
        throw InvalidDataException(InvalidDataExceptionType::CATEGORY_ID_EXISTS);
    }

    if (name.empty())
    {
        throw InvalidDataException(InvalidDataExceptionType::EMPTY_CATEGORY_NAME);
    }

    if (find_category(name).has_value())
    {
        throw InvalidDataException(InvalidDataExceptionType::CATEGORY_NAME_EXISTS);
    }

    if (id >= next_category_id)
    {
        next_category_id = id + 1;
    }

    categories.insert({ id, name });
}

void MemoryPasswordBase::remove_password(PasswordId id)
{
    const auto found_record = passwords.find(id);
    if (found_record == passwords.end())
    {
        throw InvalidDataException(InvalidDataExceptionType::UNKNOWN_PASSWORD_ID);
    }

    passwords.erase(found_record);
}

void MemoryPasswordBase::remove_category(CategoryId id)
{
    const auto found_record = categories.find(id);
    if (found_record == categories.end())
    {
        throw InvalidDataException(InvalidDataExceptionType::UNKNOWN_CATEGORY_ID);
    }

    categories.erase(found_record);

    for (auto iter = passwords.begin(); iter != passwords.end();)
    {
        if (iter->second.category_id == id)
        {
            iter = passwords.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

Password MemoryPasswordBase::get_password(PasswordId id) const
{
    const auto found_record = passwords.find(id);
    if (found_record == passwords.end())
    {
        throw InvalidDataException(InvalidDataExceptionType::UNKNOWN_PASSWORD_ID);
    }

    return found_record->second;
}

std::string MemoryPasswordBase::get_category(CategoryId id) const
{
    const auto found_record = categories.find(id);
    if (found_record == categories.end())
    {
        throw InvalidDataException(InvalidDataExceptionType::UNKNOWN_CATEGORY_ID);
    }

    return found_record->second;
}

std::vector<CategoryRecord> MemoryPasswordBase::fetch_all_categories() const
{
    std::vector<CategoryRecord> categories;
    for (auto record : this->categories)
    {
        categories.push_back(record);
    }

    return categories;
}

#include <iostream>

CategoryId MemoryPasswordBase::get_category_id(std::string name) const
{
    const auto found_id = find_category(name);
    if (!found_id.has_value())
    {
        throw UnknownCategoryNameException(name);
    }

    return found_id.value();
}

std::optional<CategoryId> MemoryPasswordBase::find_category(const std::string& name) const
{
    const auto found_pair = std::find_if(
        categories.begin(), categories.end(),
        [&](const auto& pair) { return compare_ignore_case(pair.second, name) == 0; }
    );

    if (found_pair == categories.end())
    {
        return std::nullopt;
    }
    else
    {
        return found_pair->first;
    }
}

static void sort_records(
    std::vector<PasswordRecord>& records, Comparator<Password> comparator)
{
    std::sort(
        records.begin(), records.end(),
        [&](const PasswordRecord& x, const PasswordRecord& y)
        {
            return comparator(x.second, y.second) < 0;
        }
    );
}

std::vector<PasswordRecord> MemoryPasswordBase::fetch_passwords(
    const std::vector<SearchParameter>& search_parameters,
    const std::vector<PasswordProperty>& sorting_parameters) const
{
    std::vector<PasswordRecord> fetched_passwords;
    if (!search_parameters.empty())
    {
        const auto predicate = get_searching_predicate(search_parameters);
        fetched_passwords = find_where(predicate);
    }
    else
    {
        fetched_passwords = get_all_passwords();
    }

    if (!sorting_parameters.empty())
    {
        const auto comparator = get_sorting_comparator(sorting_parameters);
        sort_records(fetched_passwords, comparator);
    }

    return fetched_passwords;
}

std::vector<PasswordRecord> MemoryPasswordBase::get_all_passwords() const
{
    std::vector<PasswordRecord> password_records;
    for (const auto& record : passwords)
    {
        password_records.push_back(record);
    }

    return password_records;
}

std::vector<PasswordRecord> MemoryPasswordBase::find_where(const Predicate<Password>& predicate) const
{
    std::vector<PasswordRecord> found;
    for (const auto& pair : passwords)
    {
        if (predicate(pair.second))
        {
            found.push_back(pair);
        }
    }

    return found;
}

Predicate<Password> MemoryPasswordBase::get_searching_predicate(
    const std::vector<SearchParameter>& parameters) const
{
    auto predicates = map<SearchParameter, Predicate<Password>>(
        parameters,
        [&](const SearchParameter& parameter)
        {
            return get_predicate(parameter);
        }
    );
    return all_of(predicates);
}

static void check_search_query_not_empty(const SearchParameter& parameter)
{
    if (parameter.query.empty())
    {
        throw InvalidFindSortParameterException(
            InvalidFindSortParameterExceptionType::EMPTY_SEARCH_QUERY, parameter.property);
    }
}

static Predicate<std::optional<std::string>> get_predicate_for_optional(const std::string& query)
{
    if (query.empty())
    {
        return [=](const std::optional<std::string>& x)
        {
            return !x.has_value();
        };
    }
    else
    {
        return [=](const std::optional<std::string>& x)
        {
            if (!x.has_value())
            {
                return false;
            }

            return contains_ignore_case(x.value(), query);
        };
    }
}

Predicate<Password> MemoryPasswordBase::get_predicate(const SearchParameter& parameter) const
{
    switch (parameter.property)
    {
    case PasswordProperty::NAME:
    {
        check_search_query_not_empty(parameter);
        const auto query = parameter.query;
        return [=](const Password& x)
        {
            return contains_ignore_case(x.name, query);
        };
    }

    case PasswordProperty::CATEGORY:
    {
        check_search_query_not_empty(parameter);
        const CategoryId category_id = get_category_id(parameter.query);
        return [=](const Password& x)
        {
            return x.category_id == category_id;
        };
    }

    case PasswordProperty::SERVICE:
    {
        const auto service_predicate = get_predicate_for_optional(parameter.query);
        return [=](const Password& x)
        {
            return service_predicate(x.service);
        };
    }

    case PasswordProperty::LOGIN:
    {
        const auto login_predicate = get_predicate_for_optional(parameter.query);
        return [=](const Password& x)
        {
            return login_predicate(x.login);
        };
    }

    default:
        throw InvalidFindSortParameterException(
            InvalidFindSortParameterExceptionType::UNKNOWN_PROPERTY, parameter.property);
    }
}

Comparator<Password> MemoryPasswordBase::get_sorting_comparator(
    const std::vector<PasswordProperty>& properties) const
{
    auto comparators = map<PasswordProperty, Comparator<Password>>(
        properties,
        [&](const PasswordProperty& property)
        {
            return get_comparator(property);
        }
    );
    return comparator_sequence(comparators);
}

static int compare_optional_strings(
    const std::optional<std::string>& x, const std::optional<std::string>& y)
{
    const bool x_has_value = x.has_value();
    const bool y_has_value = y.has_value();

    if (!x_has_value || !y_has_value)
    {
        return (int)x_has_value - (int)y_has_value;
    }

    return compare_ignore_case(x.value(), y.value());
}

Comparator<Password> MemoryPasswordBase::get_comparator(PasswordProperty property) const
{
    switch (property)
    {
    case PasswordProperty::NAME:
        return [](const Password& x, const Password& y)
        {
            return compare_ignore_case(x.name, y.name);
        };

    case PasswordProperty::CATEGORY:
        return [](const Password& x, const Password& y)
        {
            return x.category_id - y.category_id;
        };

    case PasswordProperty::SERVICE:
        return [](const Password& x, const Password& y)
        {
            return compare_optional_strings(x.service, y.service);
        };

    case PasswordProperty::LOGIN:
        return [](const Password& x, const Password& y)
        {
            return compare_optional_strings(x.login, y.login);
        };

    default:
        throw InvalidFindSortParameterException(
            InvalidFindSortParameterExceptionType::UNKNOWN_PROPERTY, property);
    }
}
