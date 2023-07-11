#include "..\..\..\include\ui\exceptions\IndexOutOfBoundsException.h"

IndexOutOfBoundsException::IndexOutOfBoundsException(unsigned int index, unsigned int list_length)
	: PrintableException("Index out of bounds"), index(index), list_length(list_length) {}

unsigned int IndexOutOfBoundsException::get_index() const
{
	return index;
}

unsigned int IndexOutOfBoundsException::get_list_length() const
{
	return list_length;
}

void IndexOutOfBoundsException::print(const ExceptionPrinter& printer) const
{
	printer.print(*this);
}
