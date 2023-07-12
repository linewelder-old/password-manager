#pragma once

#include <exception>
#include "ExceptionPrinter.h"

class PrintableException : public std::exception
{
    std::string message;

public:
	PrintableException(std::string message);

	virtual void print(const ExceptionPrinter& printer) const = 0;
    const char* what() const noexcept;
};
