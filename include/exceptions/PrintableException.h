#pragma once

#include <exception>
#include "ExceptionPrinter.h"

class PrintableException : public std::exception
{
public:
	PrintableException(std::string message);

	virtual void print(const ExceptionPrinter& printer) const = 0;
};