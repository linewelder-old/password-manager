#pragma once

#include <string>
#include "data/PasswordBase.h"
#include "exceptions/PrintableException.h"

class UnknownCategoryNameException : public PrintableException
{
    const std::string name;

public:
    UnknownCategoryNameException(std::string name);
    std::string get_category_name() const;

    void print(const ExceptionPrinter& printer) const;
};
