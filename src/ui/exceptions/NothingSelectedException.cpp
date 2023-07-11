#include "ui/exceptions/NothingSelectedException.h"

NothingSelectedException::NothingSelectedException()
    : PrintableException("Nothing selected") {}

void NothingSelectedException::print(const ExceptionPrinter& printer) const
{
    printer.print(*this);
}
