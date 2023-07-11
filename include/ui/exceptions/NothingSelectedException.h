#pragma once

#include "exceptions/PrintableException.h"

/**
* Uzytkownik nie wybral hasla i uzyl polecenia ktore wymaga wyboru hasel przez szukaj/sortuj
*/
class NothingSelectedException : public PrintableException
{
public:
    NothingSelectedException();
    void print(const ExceptionPrinter& printer) const;
};
