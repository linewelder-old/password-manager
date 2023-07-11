#include "data/exceptions/UnknownCategoryNameException.h"

UnknownCategoryNameException::UnknownCategoryNameException(std::string name)
    : PrintableException("Unknown category name"),
      name(name) {}

std::string UnknownCategoryNameException::get_category_name() const
{
    return name;
}

void UnknownCategoryNameException::print(const ExceptionPrinter& printer) const
{
    printer.print(*this);
}
