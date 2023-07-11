#include "ui/exceptions/UnknownCommandException.h"

UnknownCommandException::UnknownCommandException(std::string command)
    : PrintableException("Unknwon command"), command(command) {}

std::string UnknownCommandException::get_command() const
{
    return command;
}

void UnknownCommandException::print(const ExceptionPrinter& printer) const
{
    printer.print(*this);
}
