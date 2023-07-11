#include "util/exceptions/InvalidFileFormatException.h"

InvalidFileFormatException::InvalidFileFormatException(
    InvalidFileFormatExceptionType type)
    : PrintableException("Invalid file format"),
      type(type) {}

InvalidFileFormatExceptionType InvalidFileFormatException::get_type() const
{
    return type;
}

void InvalidFileFormatException::print(const ExceptionPrinter& printer) const
{
    printer.print(*this);
}
