#include "ui/DefaultExceptionPrinter.h"

DefaultExceptionPrinter::DefaultExceptionPrinter(std::ostream& stream)
	: stream(stream) {}

void DefaultExceptionPrinter::print(const PasswordProperty property) const
{
    switch (property)
    {
    case PasswordProperty::NAME:
        stream << "nazwa";
        break;

    case PasswordProperty::CATEGORY:
        stream << "kategoria";
        break;

    case PasswordProperty::SERVICE:
        stream << "strona";
        break;

    case PasswordProperty::LOGIN:
        stream << "login";
        break;
    }
}

void DefaultExceptionPrinter::print(const InvalidDataException& ex) const
{
    switch (ex.get_type())
    {
    case InvalidDataExceptionType::EMPTY_NAME:
        stream << "Nazwa hasla nie moze byc pusta";
        break;

    case InvalidDataExceptionType::EMPTY_PASSWORD:
        stream << "Hasla nie moze byc puste";
        break;

    case InvalidDataExceptionType::UNKNOWN_CATEGORY_ID:
        stream << "Nie znany ID kategorii";
        break;

    case InvalidDataExceptionType::CATEGORY_ID_EXISTS:
        stream << "Kategoria o takim ID juz istnieje";
        break;

    case InvalidDataExceptionType::CATEGORY_NAME_EXISTS:
        stream << "Kategoria o takiej nazwie juz istnieje";
        break;

    default:
        stream << "Nie poprawne dane";
        break;
    }
}

void DefaultExceptionPrinter::print(const InvalidFileFormatException& ex) const
{
    switch (ex.get_type())
    {
    case InvalidFileFormatExceptionType::INVALID_SIGNATURE:
        stream << "Nie jest to plik stworzony przez ten program, lub jest zepsuty";
        break;

    case InvalidFileFormatExceptionType::UNEXPECTED_EOF:
        stream << "Plik nieoczekiwanie sie skonczyl, jest on zepsuty";
        break;

    case InvalidFileFormatExceptionType::UNSUPPORTED_FILE_FORMAT_VERSION:
        stream << "Plik zostal stworzony przez inna wersje programu, go format nie podtrzymywany";
        break;

    case InvalidFileFormatExceptionType::VALUE_OUT_OF_BOUNDS:
        stream << "Wartosz poza przedzialem, plik jest zepsuty";
        break;

    default:
        stream << "Nie poprawny format pliku, jest zepsuty";
        break;
    }
}

void DefaultExceptionPrinter::print(const InvalidFindSortParameterException& ex) const
{
    switch (ex.get_type())
    {
    case InvalidFindSortParameterExceptionType::EMPTY_SEARCH_QUERY:
        stream << "Puste haslo do wyszukiwania dla pola: ";
        break;

    case InvalidFindSortParameterExceptionType::UNKNOWN_PROPERTY:
        stream << "Nie znane pole hasla: ";
        break;

    default:
        stream << "Nie znany blad w parametru dla pola: ";
        break;
    }

    print(ex.get_property());
}

void DefaultExceptionPrinter::print(const UnknownCategoryNameException& ex) const
{
    stream << "Nie znana nazwa kategorii: " << ex.get_category_name();
}

void DefaultExceptionPrinter::print(const UnexpectdCharacterException& ex) const
{
    stream << "Nieoczekiwany ";
    const char found_char = ex.get_found_char();
    if (found_char == (char)(-1))
    {
        stream << "koniec linii";
    }
    else
    {
        stream << "znak '" << found_char << '\'';
    }

    switch (ex.get_expected_character_type())
    {
    case ExpectedCharacterType::QUOTE:
        stream << ", oczekiwano '";
        break;

    case ExpectedCharacterType::DIGIT:
        stream << ", oczekiwano cyfre";
        break;

    case ExpectedCharacterType::LETTER:
        stream << ", oczekiwano litere";
        break;

    case ExpectedCharacterType::END_OF_LINE:
        stream << ", oczekiwano koniec linii";
        break;
    }
}

void DefaultExceptionPrinter::print(const UnexpectedWordException& ex) const
{
    stream << "Nieoczekiwane slowo '" << ex.get_found_word()
           << ", oczekiwano '" << ex.get_expected_word() << '\'';
}

void DefaultExceptionPrinter::print(const UnknownPasswordPropertyException& ex) const
{
    stream << "Nie znana nazwa pola hasla: " << ex.get_property();
}

void DefaultExceptionPrinter::print(const NothingSelectedException& ex) const
{
    stream << "Wykonanie tego polecenia wymaga po pierwsze wyboru hasel przez polecenie szukaj/sortuj";
}

void DefaultExceptionPrinter::print(const UnknownCommandException& ex) const
{
    stream << "Nie znane polecenie '" << ex.get_command()
           << "', napisz 'pomoc' aby zobaczyc liste dostepnych polecen";
}

void DefaultExceptionPrinter::print(const IndexOutOfBoundsException& ex) const
{
    stream << "Indeks " << ex.get_index() << " poza przedzialem 1-" << ex.get_list_length();
}
