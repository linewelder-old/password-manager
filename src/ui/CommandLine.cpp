#include "ui/CommandLine.h"

#include <iostream>
#include <sstream>
#include <map>
#include "data/PasswordGenerator.h"
#include "ui/CommandReader.h"
#include "ui/printing.h"
#include "ui/utils.h"

CommandLine::CommandLine(PasswordBase& base)
    : base(base), reader(std::cin) {}

void CommandLine::run()
{
    reader.new_line();

    while (true)
    {
        try
        {
            std::cout << " > ";
            read_and_run_command();
        }
        catch (const PrintableException& ex)
        {
            print_exception("Blad", ex);
        }

        std::cout << '\n';
    }
}

void CommandLine::read_and_run_command()
{
    reader.new_line();
    std::string command = reader.read_word();

    if (command == "szukaj")
    {
        read_and_run_find();
    }
    else if (command == "sortuj")
    {
        read_and_run_sort();
    }
    else if (command == "pokaz")
    {
        read_and_run_show();
    }
    else if (command == "edytuj")
    {
        read_and_run_edit();
    }
    else if (command == "dodaj")
    {
        std::string subcommand = reader.read_word();
        if (subcommand == "haslo")
        {
            read_and_run_add_password();
        }
        else if (subcommand == "kategorie")
        {
            read_and_run_add_category();
        }
        else
        {
            std::cout << "Blad: Nie znane polecenie: '"
                      << command << ' ' << subcommand << '\n';
        }
    }
    else if (command == "usun")
    {
        std::string subcommand = reader.read_word();
        if (subcommand == "haslo")
        {
            read_and_run_remove_password();
        }
        else if (subcommand == "kategorie")
        {
            read_and_run_remove_category();
        }
        else
        {
            std::cout << "Blad: Nie znane polecenie: '"
                      << command << ' ' << subcommand << '\n';
        }
    }
    else if (command == "pomoc")
    {
        read_and_run_help();
    }
    else
    {
        std::cout << "Blad: Nie znane polecenie: '" << command << '\n';
    }
}

std::map<std::string, PasswordProperty> password_properties
{
    { "nazwa",     PasswordProperty::NAME     },
    { "kategoria", PasswordProperty::CATEGORY },
    { "strona",    PasswordProperty::SERVICE  },
    { "login",     PasswordProperty::LOGIN    }
};

static PasswordProperty get_property_for_name(const std::string& name)
{
    const auto found = password_properties.find(name);
    if (found == password_properties.end())
    {
        throw UnknownPasswordPropertyException(name);
    }

    return found->second;
}

void CommandLine::read_and_run_find()
{
    std::vector<SearchParameter> search_for;
    while (reader.can_read_word())
    {
        const std::string property_name_or_sort = reader.read_word();
        if (property_name_or_sort == "sortuj")
        {
            read_and_run_sort(search_for);
            return;
        }

        const std::string query = reader.read_string_literal();

        const PasswordProperty property =
            get_property_for_name(property_name_or_sort);
        search_for.push_back({ property, query });
    }

    reader.read_end_of_line();

    const std::vector<PasswordProperty> sort_by;
    run_find_sort(search_for, sort_by);
}

void CommandLine::read_and_run_sort(
    const std::vector<SearchParameter>& search_for)
{
    std::vector<PasswordProperty> sort_by;
    while (reader.can_read_word())
    {
        const std::string property_name = reader.read_word();
        const PasswordProperty property = get_property_for_name(property_name);

        sort_by.push_back(property);
    }

    reader.read_end_of_line();
    run_find_sort(search_for, sort_by);
}

void CommandLine::read_and_run_show()
{
    const int index = reader.read_number();
    reader.read_end_of_line();

    const auto password = get_selected_password(index).second;
    const auto category_name = base.get_category(password.category_id);

    std::cout << "  " << password.name << '\n'
              << "Haslo: " << password.value << '\n'
              << "Kategoria: " << category_name << '\n';

    if (password.service.has_value())
    {
        std::cout << "Strona: " << password.service.value() << '\n';
    }

    if (password.login.has_value())
    {
        std::cout << "Login: " << password.login.value() << '\n';
    }
}

void CommandLine::read_and_run_edit()
{
    std::cout << "Nie zaimplementowano :(\n";
    reader.read_end_of_line();
}

PasswordRecord CommandLine::get_selected_password(unsigned int index)
{
    if (selected_passwords.empty())
    {
        throw NothingSelectedException();
    }

    if (index == 0 || index > selected_passwords.size())
    {
        throw IndexOutOfBoundsException(index, selected_passwords.size());
    }

    return selected_passwords.at(index - 1);
}

void CommandLine::read_and_run_add_password()
{
    const std::string name = reader.read_string_literal();

    std::optional<std::string> service = std::nullopt;
    std::optional<std::string> login = std::nullopt;

    while (reader.can_read_word())
    {
        std::string property_name = reader.read_word();
        if (property_name == "strona")
        {
            service = reader.read_string_literal();
        }
        else if (property_name == "login")
        {
            login = reader.read_string_literal();
        }
        else
        {
            throw UnknownPasswordPropertyException(property_name);
        }
    }

    reader.read_end_of_line();

    CategoryId category_id;
    const auto categories = base.fetch_all_categories();
    if (categories.empty())
    {
        std::string new_category;
        std::cout << "  Wpisz nazwe nowej kategorii:\n > ";
        std::cin >> new_category;

        category_id = base.add_category(new_category);
    }
    else
    {
        std::cout << "  Wybierz kategorie:\n";

        print_list<CategoryRecord>(
            categories,
            [](const CategoryRecord& x) { return x.second; }
        );

        unsigned int index;
        while (true)
        {
            std::cout << " > ";
            
            reader.new_line();
            index = reader.read_number();

            if (index > 0 && index <= categories.size())
            {
                break;
            }

            std::cout << "Blad: Indeks jest poza przedzialem 1-" << categories.size() << '\n';
        }
        
        category_id = categories.at(index - 1).first;
    }

    std::cout << "  Wpisz wartosc hasla (lub wpisz polecenie 'generuj' dla generacji hasla):\n"
              << " > ";

    std::string value;
    std::cin >> value;

    if (value == "generuj")
    {
        std::cout << "  Wpisz dlugosc hasla i typy znakow:\n > ";
        reader.new_line();
        reader.new_line();
        unsigned int password_length = reader.read_number();
        
        PasswordGenerator generator;
        while (reader.can_read_word())
        {
            std::string char_set = reader.read_word();
            if (char_set == "duze")
            {
                generator.use_uppercase = true;
            }
            else if (char_set == "male")
            {
                generator.use_lowercase = true;
            }
            else if (char_set == "cyfry")
            {
                generator.use_digits = true;
            }
            else if (char_set == "znaki")
            {
                generator.use_special_characters = true;
            }
            else
            {
                std::cout << "Blad: Nie znana kategoria znakow: " << char_set << '\n';
                return;
            }
        }
        reader.read_end_of_line();

        const auto maybe_value = generator.generate(password_length);
        if (!maybe_value.has_value())
        {
            std::cout << "Blad: Nie wybrano zadnej kategorii znakow\n";
            return;
        }

        value = maybe_value.value();
        std::cout << "  Wygenerowano: " << value << '\n';
    }

    base.add_password({ name, value, category_id, service, login });
}

void CommandLine::read_and_run_remove_password()
{
    const int index = reader.read_number();
    reader.read_end_of_line();

    const auto password_id = get_selected_password(index).first;
    base.remove_password(password_id);
}

void CommandLine::read_and_run_add_category()
{
    const std::string name = reader.read_string_literal();
    reader.read_end_of_line();

    base.add_category(name);
}

void CommandLine::read_and_run_remove_category()
{
    const std::string name = reader.read_string_literal();
    reader.read_end_of_line();

    CategoryId id = base.get_category_id(name);
    base.remove_category(id);
}

void CommandLine::read_and_run_help()
{
    std::cout << "Nie zaimplementowano :(\n";
    reader.read_end_of_line();
}

void CommandLine::run_find_sort(
    const std::vector<SearchParameter>& search_for,
    const std::vector<PasswordProperty>& sort_by)
{
    const auto categories = base.fetch_all_categories();
    selected_passwords = base.fetch_passwords(search_for, sort_by);
    print_password_table(categories, selected_passwords);
}
