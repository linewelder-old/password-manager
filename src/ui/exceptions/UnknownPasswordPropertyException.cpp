#include "ui/exceptions/UnknownPasswordPropertyException.h"

UnknownPasswordPropertyException::UnknownPasswordPropertyException(std::string property)
	: PrintableException("Unknown password property") {}

std::string UnknownPasswordPropertyException::get_property() const
{
	return property;
}

void UnknownPasswordPropertyException::print(const ExceptionPrinter& printer) const
{
	printer.print(*this);
}
