#pragma once

#include "data/PasswordBase.h"
#include "exceptions/PrintableException.h"

enum class InvalidFindSortParameterExceptionType
{
    EMPTY_SEARCH_QUERY,
    UNKNOWN_PROPERTY
};

class InvalidFindSortParameterException : public PrintableException
{
    const InvalidFindSortParameterExceptionType type;
    const PasswordProperty property;

public:
    InvalidFindSortParameterException(
        InvalidFindSortParameterExceptionType type, PasswordProperty property);

    InvalidFindSortParameterExceptionType get_type() const;
    PasswordProperty get_property() const;

    void print(const ExceptionPrinter& printer) const override;
};
