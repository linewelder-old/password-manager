#pragma once

#include "data/PasswordBase.h"
#include "exceptions/PrintableException.h"

enum class InvalidDataExceptionType
{
    EMPTY_NAME,
    EMPTY_PASSWORD,
    EMPTY_CATEGORY_NAME,
    CATEGORY_ID_EXISTS,
    CATEGORY_NAME_EXISTS,
    UNKNOWN_CATEGORY_ID,
    UNKNOWN_PASSWORD_ID
};

class InvalidDataException : public PrintableException
{
    const InvalidDataExceptionType type;

public:
    InvalidDataException(InvalidDataExceptionType type);
    InvalidDataExceptionType get_type() const;

    void print(const ExceptionPrinter& printer) const override;
};
