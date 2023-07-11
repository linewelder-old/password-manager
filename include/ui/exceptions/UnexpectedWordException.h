#pragma once

#include "exceptions/PrintableException.h"

class UnexpectedWordException : public PrintableException
{
    const std::string found_word;
    const std::string expected_word;

public:
    UnexpectedWordException(std::string expected_word, std::string found_word);
    std::string get_found_word() const;
    std::string get_expected_word() const;

    void print(const ExceptionPrinter& printer) const;
};
