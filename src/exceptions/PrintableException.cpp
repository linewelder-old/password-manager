#include "exceptions/PrintableException.h"

#include <exception>

PrintableException::PrintableException(std::string what)
    : std::exception(what.c_str()) {}
