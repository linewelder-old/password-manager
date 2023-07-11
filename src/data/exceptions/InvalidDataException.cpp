#include "data/exceptions/InvalidDataException.h"

InvalidDataException::InvalidDataException(
    InvalidDataExceptionType type)
    : PrintableException("Invalid data"),
      type(type) {}

InvalidDataExceptionType InvalidDataException::get_type() const
{
    return type;
}

void InvalidDataException::print(const ExceptionPrinter& printer) const
{
    printer.print(*this);
}
