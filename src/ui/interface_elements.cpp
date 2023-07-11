#include "ui/interface_elements.h"

#include <iostream>
#include <filesystem>
#include "ui/printing.h"
#include "ui/utils.h"

namespace fs = std::filesystem;

fs::path select_file(std::string file_extension)
{
    const fs::path current_directory = fs::current_path();
    const auto base_files = get_files_with_extension(
        current_directory, file_extension);

    if (!base_files.empty())
    {
        std::cout
            << "  Wybierz baze hasel, albo wprowadz plik do zaimporowania:\n"
            << "  (albo wpisz nazwe nowej bazy dla stworzenia)\n";

        print_list<fs::path>(
            base_files,
            [](const fs::path& x) { return get_stripped_filename(x); }
        );
    }
    else
    {
        std::cout
            << "  Wpisz nazwe nowej bazy, albo wprowadz plik do zaimporowania:\n";
    }

    while (true)
    {
        std::string input;
        std::cout << " > ";
        std::cin >> input;

        const auto maybe_list_index = try_read_int(input);
        if (maybe_list_index.has_value())
        {
            const int list_index = maybe_list_index.value();
            if (base_files.size() == 0)
            {
                std::cout << "Blad: Nie mozna wybrac bazy z listy, bo lista pusta\n";
                continue;
            }
            if (list_index <= 0 || list_index > base_files.size())
            {
                std::cout << "Blad: Wprowadzony indeks jest poza przedzialem 1-" << base_files.size() << '\n';
                continue;
            }

            return base_files.at(list_index - 1);
        }
        else if (is_considered_path(input))
        {
            if (fs::path(input).extension() != file_extension)
            {
                std::cout << "Blad: Plik musi byc rozszezenia '" << file_extension << "'\n";
                continue;
            }
            if (!fs::exists(input))
            {
                std::cout << "Blad: Plik nie istnieje\n";
                continue;
            }
            if (!fs::is_regular_file(input))
            {
                std::cout << "Blad: Nie jest to plikiem\n";
                continue;
            }

            const fs::path filename = fs::path(input).filename();
            const fs::path imported_path = current_directory / filename;
            if (fs::exists(imported_path))
            {
                std::cout << "Blad: Baza o takiej nazwie juz istnieje\n";
                continue;
            }

            fs::copy_file(input, imported_path);
            return imported_path;
        }
        else
        {
            const fs::path filename = fs::path(input).replace_extension(file_extension);
            const fs::path filepath = current_directory / filename;
            if (fs::exists(filepath))
            {
                std::cout << "Blad: Baza o takiej nazwie juz istnieje. Aby ja otworzyc wprowadz jej numer z listy\n";
                continue;
            }

            return filepath;
        }
    }
}

std::string ask_password()
{
    std::string password;
    std::cout << " Haslo od bazy: ";
    std::cin >> password;
    return password;
}
