#include "data/exceptions/InvalidFindSortParameterException.h"

InvalidFindSortParameterException::InvalidFindSortParameterException(
    InvalidFindSortParameterExceptionType type, PasswordProperty property)
    : PrintableException("Invalid find/sort parameter"),
      type(type), property(property) {}

InvalidFindSortParameterExceptionType InvalidFindSortParameterException::get_type() const
{
    return type;
}

PasswordProperty InvalidFindSortParameterException::get_property() const
{
    return property;
}

void InvalidFindSortParameterException::print(const ExceptionPrinter& printer) const
{
    printer.print(*this);
}
