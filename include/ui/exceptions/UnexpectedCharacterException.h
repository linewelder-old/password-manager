#pragma once

#include "data/PasswordBase.h"
#include "exceptions/PrintableException.h"

enum class ExpectedCharacterType
{
    LETTER,
    DIGIT,
    QUOTE,
    END_OF_LINE
};

class UnexpectdCharacterException : public PrintableException
{
    const ExpectedCharacterType expected_character_type;
    const char found_char;

public:
    UnexpectdCharacterException(ExpectedCharacterType expected_character_type,
        char found_char);
    ExpectedCharacterType get_expected_character_type() const;
    char get_found_char() const;

    void print(const ExceptionPrinter& printer) const override;
};
