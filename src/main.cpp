#include <iostream>
#include <filesystem>
#include "data/Password.h"
#include "data/MemoryPasswordBase.h"
#include "data/FilePasswordBase.h"
#include "util/exceptions/InvalidFileFormatException.h"
#include "exceptions/PrintableException.h"
#include "ui/DefaultExceptionPrinter.h"
#include "ui/CommandLine.h"
#include "ui/printing.h"
#include "ui/utils.h"
#include "ui/interface_elements.h"

namespace fs = std::filesystem;

const char* TITLE =
" _   _   ____   ____   _      _  _    ____   ____   ____   _____   _   __\n"
"| | | | |    | |  __| | | _  / \\/ \\  |    | |__  | |    | |_   _| | | / /\n"
"| |_| | | || | | |__  | |// | ^  ^ | | || |    | | | || |   | |   | |/ /\n"
"|  _  | |    | |__  | |  /   \\ \\/ /  |  __|  _ | | |    |   | |   |   |\n"
"| | | | | || |  __| | | |___  \\  /   | |    | \\/ | | || |   | |   | |\\ \\\n"
"|_| |_| |_||_| |____| |_____|  \\/    |_|     \\__/  |_||_|   |_|   |_| \\_\\\n";

const char* FILE_EXTENSION = ".hpb";

MemoryPasswordBase memory_base;
std::unique_ptr<PasswordBase> base;

void open_base(fs::path file, std::string password)
{
    const std::string base_name = get_stripped_filename(file);

    if (!fs::exists(file))
    {
        std::cout << "Stworzono baze: " << base_name << "\n\n";
        base = std::make_unique<FilePasswordBase>(
            FilePasswordBase::create_new(file, password, memory_base));
    }
    else
    {
        std::cout << "Wybrana baza: " << base_name << '\n';
        std::cout << "Odszyfrowanie bazy... ";

        FilePasswordBase file_base = FilePasswordBase::load_from_file(file, password, memory_base);

        std::cout << "Gotowe\n";

        time_t last_opening_time = file_base.get_last_opening_time();
        std::cout << "Ostatnia interakcja: ";
        print_time(last_opening_time);
        std::cout << "\n\n";

        base = std::make_unique<FilePasswordBase>(file_base);
    }
}

void select_and_open_base()
{
    while (true)
    {
        try
        {
            const fs::path file = select_file(FILE_EXTENSION);
            const std::string password = ask_password();
            std::cout << '\n';

            open_base(file, password);
            break;
        }
        catch (const PrintableException& ex)
        {
            std::cout << '\n';
            print_exception("Nie dalo sie odczytac dane", ex);
            std::cout << '\n';
        }
    }
}

int main()
{
    srand(time(0));

    std::cout << TITLE << '\n';
    select_and_open_base();

    CommandLine command_line(*base);
    command_line.run();

    return 0;
}
