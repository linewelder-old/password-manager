#pragma once

#include "exceptions/PrintableException.h"

class UnknownPasswordPropertyException : public PrintableException
{
    const std::string property;

public:
    UnknownPasswordPropertyException(std::string property);
    std::string get_property() const;

    void print(const ExceptionPrinter& printer) const;
};
