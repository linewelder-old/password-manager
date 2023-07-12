#include "exceptions/PrintableException.h"

#include <exception>

PrintableException::PrintableException(std::string what)
    : message(what) {}

const char* PrintableException::what() const noexcept
{
    return message.c_str();
}
