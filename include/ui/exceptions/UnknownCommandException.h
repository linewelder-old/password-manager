#pragma once

#include "exceptions/PrintableException.h"

class UnknownCommandException : public PrintableException
{
    const std::string command;

public:
    UnknownCommandException(std::string command);
    std::string get_command() const;

    void print(const ExceptionPrinter& printer) const override;
};
