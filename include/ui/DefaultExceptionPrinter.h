#pragma once

#include <iostream>
#include "data/PasswordBase.h"
#include "data/exceptions/InvalidDataException.h"
#include "data/exceptions/InvalidFindSortParameterException.h"
#include "data/exceptions/UnknownCategoryNameException.h"
#include "util/exceptions/InvalidFileFormatException.h"
#include "ui/exceptions/UnexpectedCharacterException.h"
#include "ui/exceptions/UnexpectedWordException.h"
#include "ui/exceptions/UnknownPasswordPropertyException.h"
#include "ui/exceptions/UnknownCommandException.h"
#include "ui/exceptions/IndexOutOfBoundsException.h"
#include "ui/exceptions/NothingSelectedException.h"
#include "exceptions/ExceptionPrinter.h"

class DefaultExceptionPrinter : public ExceptionPrinter
{
    std::ostream& stream;

public:
    DefaultExceptionPrinter(std::ostream& stream);

    void print(const PasswordProperty property) const;

    void print(const InvalidDataException&              ex) const override;
    void print(const InvalidFileFormatException&        ex) const override;
    void print(const InvalidFindSortParameterException& ex) const override;
    void print(const UnknownCategoryNameException&      ex) const override;
    void print(const UnexpectdCharacterException&       ex) const override;
    void print(const UnexpectedWordException&           ex) const override;
    void print(const UnknownPasswordPropertyException&  ex) const override;
    void print(const NothingSelectedException&          ex) const override;
    void print(const UnknownCommandException&           ex) const override;
    void print(const IndexOutOfBoundsException&         ex) const override;
};
