#include "ui/exceptions/UnexpectedWordException.h"
#include "ui/exceptions/UnknownPasswordPropertyException.h"

UnexpectedWordException::UnexpectedWordException(
	std::string expected_word, std::string found_word)
	: PrintableException("Unexpected word"),
	  found_word(found_word), expected_word(expected_word) {}

std::string UnexpectedWordException::get_found_word() const
{
	return found_word;
}

std::string UnexpectedWordException::get_expected_word() const
{
	return expected_word;
}

void UnexpectedWordException::print(const ExceptionPrinter& printer) const
{
	printer.print(*this);
}
