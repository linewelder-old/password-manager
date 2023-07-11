#pragma once

#include <iostream>

enum class PasswordProperty;
class InvalidDataException;
class InvalidFileFormatException;
class InvalidFindSortParameterException;
class UnknownCategoryNameException;
class UnexpectdCharacterException;
class UnexpectedWordException;
class UnknownPasswordPropertyException;
class NothingSelectedException;
class UnknownCommandException;
class IndexOutOfBoundsException;

class ExceptionPrinter
{
public:
    virtual void print(const PasswordProperty property) const = 0;

    virtual void print(const InvalidDataException&              ex) const = 0;
    virtual void print(const InvalidFileFormatException&        ex) const = 0;
    virtual void print(const InvalidFindSortParameterException& ex) const = 0;
    virtual void print(const UnknownCategoryNameException&      ex) const = 0;
    virtual void print(const UnexpectdCharacterException&       ex) const = 0;
    virtual void print(const UnexpectedWordException&           ex) const = 0;
    virtual void print(const UnknownPasswordPropertyException&  ex) const = 0;
    virtual void print(const NothingSelectedException&          ex) const = 0;
    virtual void print(const UnknownCommandException&           ex) const = 0;
    virtual void print(const IndexOutOfBoundsException&         ex) const = 0;
};
