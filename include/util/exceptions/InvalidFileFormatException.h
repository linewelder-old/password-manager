#pragma once

#include "data/PasswordBase.h"
#include "exceptions/PrintableException.h"

enum class InvalidFileFormatExceptionType
{
    UNEXPECTED_EOF,
    INVALID_SIGNATURE,
    UNSUPPORTED_FILE_FORMAT_VERSION,
    VALUE_OUT_OF_BOUNDS
};

class InvalidFileFormatException : public PrintableException
{
    const InvalidFileFormatExceptionType type;

public:
    InvalidFileFormatException(InvalidFileFormatExceptionType type);
    InvalidFileFormatExceptionType get_type() const;

    void print(const ExceptionPrinter& printer) const;
};
