#include "ui/exceptions/UnexpectedCharacterException.h"

UnexpectdCharacterException::UnexpectdCharacterException(
	ExpectedCharacterType expected_character_type, char found_char)
	: PrintableException("Unexpected character"),
	  expected_character_type(expected_character_type),
	  found_char(found_char) {}

ExpectedCharacterType UnexpectdCharacterException::get_expected_character_type() const
{
	return expected_character_type;
}

char UnexpectdCharacterException::get_found_char() const
{
	return found_char;
}

void UnexpectdCharacterException::print(const ExceptionPrinter& printer) const
{
	printer.print(*this);
}
