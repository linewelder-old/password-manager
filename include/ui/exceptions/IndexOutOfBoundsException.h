#pragma once

#include "exceptions/PrintableException.h"

class IndexOutOfBoundsException : public PrintableException
{
    const unsigned int index;
    const unsigned int list_length;

public:
    IndexOutOfBoundsException(unsigned int index, unsigned int list_length);
    unsigned int get_index() const;
    unsigned int get_list_length() const;

    void print(const ExceptionPrinter& printer) const override;
};
